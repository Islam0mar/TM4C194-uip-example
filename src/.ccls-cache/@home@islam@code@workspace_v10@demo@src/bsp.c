/*
 * bsp.c
 *
 *  Created on: Dec 31, 2020
 *      Author: islam
 */

#include "bsp.h"

#include "config.h"
/* config has be first */

#include <stdbool.h>
#include <stdint.h>

#include "driverlib/emac.h"
#include "driverlib/flash.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "inc/hw_emac.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
/* #include "inc/tm4c1294ncpdt.h" */
#include "logger.h"
#include "states.h"

uint32_t g_ui32SysClock;
static uint32_t g_ui32TickCounter;
uint32_t g_ui32Flags;

/* min is 3 from TivaWare™ Peripheral Driver Library USER’S GUIDE
 Copyright © 2006-2020Texas Instruments Incorporated
SW-TM4C-DRL-UG-2.2.0.295March 2013 - Revised April 2020 */
#define NUM_TX_DESCRIPTORS 3
#define NUM_RX_DESCRIPTORS 3
tEMACDMADescriptor g_psRxDescriptor[NUM_TX_DESCRIPTORS];
tEMACDMADescriptor g_psTxDescriptor[NUM_RX_DESCRIPTORS];

uint32_t g_ui32RxDescIndex;
uint32_t g_ui32TxDescIndex;

/* Transmit and receive buffers. */
#define RX_BUFFER_SIZE 1536
#define TX_BUFFER_SIZE 1536
uint8_t g_pui8RxBuffer[RX_BUFFER_SIZE];
uint8_t g_pui8TxBuffer[TX_BUFFER_SIZE];

/* ISR for ethernet */
void EthernetIntHandler(void) {
  uint32_t ui32Temp;

  /* Read and Clear the interrupt. */
  ui32Temp = MAP_EMACIntStatus(EMAC0_BASE, true);
  MAP_EMACIntClear(EMAC0_BASE, ui32Temp);

  /* Check to see if an RX Interrupt has occurred. */
  if (ui32Temp & EMAC_INT_RECEIVE) {
    /* Indicate that a packet has been received. */
    HWREGBITW(&g_ui32Flags, kEmacReceivedFrame) = 1;
  }

  /* Has the DMA finished transferring a packet to the transmitter? */
  if (ui32Temp & EMAC_INT_TRANSMIT) {
    /* Indicate that a packet has been sent. */
    HWREGBITW(&g_ui32Flags, kEmacTransferredFrame) = 1;
  }
}
/**
 * @brief      function description
 *
 * @details    detailed description
 *
 * @param      param
 *
 * @return     return type
 */
int32_t BspPacketReceive(uint8_t *pui8Buf, int32_t i32BufLen) {
  int_fast32_t i32FrameLen, i32Loop;
  ASSERT(pui8Buf != 0);
  ASSERT(i32BufLen > 0);

  /* By default, we assume we got a bad frame. */
  i32FrameLen = 0;

  // Make sure that we own the receive descriptor.
  if (!(g_psRxDescriptor[g_ui32RxDescIndex].ui32CtrlStatus &
        DES0_RX_CTRL_OWN)) {
    // We own the receive descriptor so check to see if it contains a valid
    // frame.  Look for a descriptor error, indicating that the incoming
    // packet was truncated or, if this is the last frame in a packet,
    // the receive error bit.
    if (!(g_psRxDescriptor[g_ui32RxDescIndex].ui32CtrlStatus &
          DES0_RX_STAT_ERR)) {
      // We have a valid frame so copy the content to the supplied
      // buffer. First check that the "last descriptor" flag is set.  We
      // sized the receive buffer such that it can always hold a valid
      // frame so this flag should never be clear at this point but...
      if (g_psRxDescriptor[g_ui32RxDescIndex].ui32CtrlStatus &
          DES0_RX_STAT_LAST_DESC) {
        i32FrameLen = ((g_psRxDescriptor[g_ui32RxDescIndex].ui32CtrlStatus &
                        DES0_RX_STAT_FRAME_LENGTH_M) >>
                       DES0_RX_STAT_FRAME_LENGTH_S);

        // Sanity check.  This shouldn't be required since we sized the
        // uIP buffer such that it's the same size as the DMA receive
        // buffer but, just in case...
        if (i32FrameLen > i32BufLen) {
          i32FrameLen = i32BufLen;
        }

        // Copy the data from the DMA receive buffer into the provided
        // frame buffer.
        for (i32Loop = 0; i32Loop < i32FrameLen; i32Loop++) {
          pui8Buf[i32Loop] = g_pui8RxBuffer[i32Loop];
        }
      }
    }

    // Move on to the next descriptor in the chain.
    g_ui32RxDescIndex++;
    if (g_ui32RxDescIndex == NUM_RX_DESCRIPTORS) {
      g_ui32RxDescIndex = 0;
    }

    // Mark the next descriptor in the ring as available for the receiver
    // to write into.
    g_psRxDescriptor[g_ui32RxDescIndex].ui32CtrlStatus = DES0_RX_CTRL_OWN;
  }

  // Return the Frame Length
  //
  return (i32FrameLen);
}

// Transmit a packet from the supplied buffer.
int32_t BspPacketTransmit(uint8_t *pui8Buf, int32_t i32BufLen) {
  int_fast32_t i32Loop;

  // Indicate that a packet is being sent.
  HWREGBITW(&g_ui32Flags, kEmacTransferredFrame) = 0;

  // Wait for the previous packet to be transmitted.
  while (g_psTxDescriptor[g_ui32TxDescIndex].ui32CtrlStatus &
         DES0_TX_CTRL_OWN) {
    // Spin and waste time.
  }

  // Check that we're not going to overflow the transmit buffer.  This
  // shouldn't be necessary since the uIP buffer is smaller than our DMA
  // transmit buffer but, just in case...
  if (i32BufLen > TX_BUFFER_SIZE) {
    i32BufLen = TX_BUFFER_SIZE;
  }

  // Copy the packet data into the transmit buffer.
  for (i32Loop = 0; i32Loop < i32BufLen; i32Loop++) {
    g_pui8TxBuffer[i32Loop] = pui8Buf[i32Loop];
  }

  // Move to the next descriptor.
  g_ui32TxDescIndex++;
  if (g_ui32TxDescIndex == NUM_TX_DESCRIPTORS) {
    g_ui32TxDescIndex = 0;
  }

  // Fill in the packet size and tell the transmitter to start work.
  g_psTxDescriptor[g_ui32TxDescIndex].ui32Count = (uint32_t)i32BufLen;
  g_psTxDescriptor[g_ui32TxDescIndex].ui32CtrlStatus =
      (DES0_TX_CTRL_LAST_SEG | DES0_TX_CTRL_FIRST_SEG | DES0_TX_CTRL_INTERRUPT |
       DES0_TX_CTRL_IP_ALL_CKHSUMS | DES0_TX_CTRL_CHAINED | DES0_TX_CTRL_OWN);

  // Tell the DMA to reacquire the descriptor now that we've filled it in.
  MAP_EMACTxDMAPollDemand(EMAC0_BASE);

  // Return the number of bytes sent.
  return (i32BufLen);
}

//*****************************************************************************
// Initialize the transmit and receive DMA descriptors.  We apparently need
// a minimum of 3 descriptors in each chain.  This is overkill since uIP uses
// a single, common transmit and receive buffer so we tag each descriptor
// with the same buffer and will make sure we only hand the DMA one descriptor
// at a time.
//*****************************************************************************
void InitDescriptors(uint32_t ui32Base) {
  uint32_t ui32Loop;

  // Initialize each of the transmit descriptors.  Note that we leave the OWN
  // bit clear here since we have not set up any transmissions yet.
  for (ui32Loop = 0; ui32Loop < NUM_TX_DESCRIPTORS; ui32Loop++) {
    g_psTxDescriptor[ui32Loop].ui32Count =
        (DES1_TX_CTRL_SADDR_INSERT |
         (TX_BUFFER_SIZE << DES1_TX_CTRL_BUFF1_SIZE_S));
    g_psTxDescriptor[ui32Loop].pvBuffer1 = g_pui8TxBuffer;
    g_psTxDescriptor[ui32Loop].DES3.pLink =
        (ui32Loop == (NUM_TX_DESCRIPTORS - 1))
            ? g_psTxDescriptor
            : &g_psTxDescriptor[ui32Loop + 1];
    g_psTxDescriptor[ui32Loop].ui32CtrlStatus =
        (DES0_TX_CTRL_LAST_SEG | DES0_TX_CTRL_FIRST_SEG |
         DES0_TX_CTRL_INTERRUPT | DES0_TX_CTRL_CHAINED |
         DES0_TX_CTRL_IP_ALL_CKHSUMS);
  }

  // Initialize each of the receive descriptors.  We clear the OWN bit here
  // to make sure that the receiver doesn't start writing anything
  // immediately.
  for (ui32Loop = 0; ui32Loop < NUM_RX_DESCRIPTORS; ui32Loop++) {
    g_psRxDescriptor[ui32Loop].ui32CtrlStatus = 0;
    g_psRxDescriptor[ui32Loop].ui32Count =
        (DES1_RX_CTRL_CHAINED | (RX_BUFFER_SIZE << DES1_RX_CTRL_BUFF1_SIZE_S));
    g_psRxDescriptor[ui32Loop].pvBuffer1 = g_pui8RxBuffer;
    g_psRxDescriptor[ui32Loop].DES3.pLink =
        (ui32Loop == (NUM_RX_DESCRIPTORS - 1))
            ? g_psRxDescriptor
            : &g_psRxDescriptor[ui32Loop + 1];
  }

  // Set the descriptor pointers in the hardware.
  MAP_EMACRxDMADescriptorListSet(ui32Base, g_psRxDescriptor);
  MAP_EMACTxDMADescriptorListSet(ui32Base, g_psTxDescriptor);

  // Start from the beginning of both descriptor chains.  We actually set
  // the transmit descriptor index to the last descriptor in the chain
  // since it will be incremented before use and this means the first
  // transmission we perform will use the correct descriptor.
  g_ui32RxDescIndex = 0;
  g_ui32TxDescIndex = NUM_TX_DESCRIPTORS - 1;
}

uint8_t *BspGetMacAddress() {
  static uint8_t mac_addr[6];
  return mac_addr;
}

uint8_t EMacInit() {
  uint32_t ui32User0, ui32User1;
  uint32_t ui32PHYConfig;
  uint8_t *mac_addr = BspGetMacAddress();

  LOG_INFO("Using Internal PHY.");
  ui32PHYConfig = (EMAC_PHY_TYPE_INTERNAL | EMAC_PHY_INT_MDIX_EN |
                   EMAC_PHY_AN_100B_T_FULL_DUPLEX);

  // Read the MAC address from the user registers.
  MAP_FlashUserGet(&ui32User0, &ui32User1);
  if ((ui32User0 == 0xffffffff) || (ui32User1 == 0xffffffff)) {
    // We should never get here.  This is an error if the MAC address has
    // not been programmed into the device.  Exit the program.
    LOG_INFO("MAC Address Not Programmed!");
    while (1) {
    }
  }

  // Enable and reset the Ethernet modules.
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_EMAC0);
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_EPHY0);
  MAP_SysCtlPeripheralReset(SYSCTL_PERIPH_EMAC0);
  MAP_SysCtlPeripheralReset(SYSCTL_PERIPH_EPHY0);

  // Wait for the MAC to be ready.
  LOG_INFO("Waiting for MAC to be ready...");
  while (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_EMAC0)) {
  }

  // Configure for use with the internal PHY.
  MAP_EMACPHYConfigSet(EMAC0_BASE, ui32PHYConfig);
  LOG_INFO("MAC ready.");

  // Reset the MAC.
  MAP_EMACReset(EMAC0_BASE);

  // Initialize the MAC and set the DMA mode.
  MAP_EMACInit(EMAC0_BASE, g_ui32SysClock,
               EMAC_BCONFIG_MIXED_BURST | EMAC_BCONFIG_PRIORITY_FIXED, 4, 4, 0);

  // Set MAC configuration options.
  MAP_EMACConfigSet(
      EMAC0_BASE,
      (EMAC_CONFIG_FULL_DUPLEX | EMAC_CONFIG_CHECKSUM_OFFLOAD |
       EMAC_CONFIG_7BYTE_PREAMBLE | EMAC_CONFIG_IF_GAP_96BITS |
       EMAC_CONFIG_USE_MACADDR0 | EMAC_CONFIG_SA_FROM_DESCRIPTOR |
       EMAC_CONFIG_BO_LIMIT_1024),
      (EMAC_MODE_RX_STORE_FORWARD | EMAC_MODE_TX_STORE_FORWARD |
       EMAC_MODE_TX_THRESHOLD_64_BYTES | EMAC_MODE_RX_THRESHOLD_64_BYTES),
      0);

  // Initialize the Ethernet DMA descriptors.
  InitDescriptors(EMAC0_BASE);

  mac_addr[0] = ((ui32User0 >> 0) & 0xff);
  mac_addr[1] = ((ui32User0 >> 8) & 0xff);
  mac_addr[2] = ((ui32User0 >> 16) & 0xff);
  mac_addr[3] = ((ui32User1 >> 0) & 0xff);
  mac_addr[4] = ((ui32User1 >> 8) & 0xff);
  mac_addr[5] = ((ui32User1 >> 16) & 0xff);
  //
  // Program the hardware with its MAC address (for filtering).
  //
  MAP_EMACAddrSet(EMAC0_BASE, 0, mac_addr);

  //
  // Wait for the link to become active.
  //
  LOG_INFO("Waiting for Link.");
  while ((MAP_EMACPHYRead(EMAC0_BASE, 0, EPHY_BMSR) & EPHY_BMSR_LINKSTAT) ==
         0) {
  }

  LOG_INFO("Link Established.");

  // Set MAC filtering options.  We receive all broadcast and multicast
  // packets along with those addressed specifically for us.
  MAP_EMACFrameFilterSet(EMAC0_BASE,
                         (EMAC_FRMFILTER_SADDR | EMAC_FRMFILTER_PASS_MULTICAST |
                          EMAC_FRMFILTER_PASS_NO_CTRL));

  // Clear any pending interrupts.
  MAP_EMACIntClear(EMAC0_BASE, EMACIntStatus(EMAC0_BASE, false));

  // Enable the Ethernet MAC transmitter and receiver.
  MAP_EMACTxEnable(EMAC0_BASE);
  MAP_EMACRxEnable(EMAC0_BASE);

  // Enable the Ethernet interrupt.
  MAP_IntEnable(INT_EMAC0);

  // Enable the Ethernet RX Packet interrupt source.
  MAP_EMACIntEnable(EMAC0_BASE, EMAC_INT_RECEIVE);

  // Mark the first receive descriptor as available to the DMA to start
  // the receive processing.
  g_psRxDescriptor[g_ui32RxDescIndex].ui32CtrlStatus |= DES0_RX_CTRL_OWN;

  return true;
}

void __error__(char *pcFilename, uint32_t ui32Line) {
  /* NVIC_SystemReset(); /\* reset the system *\/ */
  while (1)
    ;
}
//*****************************************************************************
// The interrupt handler for the SysTick interrupt.
//*****************************************************************************
void SysTickIntHandler(void) {
  // Increment the system tick count.
  g_ui32TickCounter++;
}
//*****************************************************************************
// When using the timer module in UIP, this function is required to return
// the number of ticks.  Note that the file "clock-arch.h" must be provided
// by the application, and define CLOCK_CONF_SECONDS as the number of ticks
// per second, and must also define the typedef "clock_time_t".
//*****************************************************************************
clock_time_t clock_time(void) {
  // Increment the system tick count.
  return ((clock_time_t)g_ui32TickCounter);
}
void BspIrqOn() { IntMasterEnable(); }
void BspIrqOff() { IntMasterDisable(); }
uint32_t BspGetMSecTime() { return g_ui32TickCounter; }

uint8_t BspInit() {
  /*  Run from the PLL at 120 MHz.*/
  g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                           SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                          120000000);

  /* Make sure the main oscillator is enabled because this is required by */
  /* the PHY.  The system must have a 25MHz crystal attached to the OSC */
  /* pins. The SYSCTL_MOSC_HIGHFREQ parameter is used when the crystal */
  /* frequency is 10MHz or higher. */

  SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);
  /* Enable all the GPIO peripherals. */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);

  /* use high speed GPIO */
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOA);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOB);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOC);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOD);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOE);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOF);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOG);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOH);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOJ);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOK);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOL);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOM);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPION);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOP);
  SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOQ);

  /* Keep the default config for most pins used by USB. */
  /* Add a pull down to PD6 to turn off the TPS2052 switch */
  GPIOPinTypeGPIOInput(GPIO_PORTD_AHB_BASE, GPIO_PIN_6);
  GPIOPadConfigSet(GPIO_PORTD_AHB_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA,
                   GPIO_PIN_TYPE_STD_WPD);

  /* this app wants to configure for ethernet LED function. */
  GPIOPinConfigure(GPIO_PF0_EN0LED0);
  GPIOPinConfigure(GPIO_PF4_EN0LED1);
  GPIOPinTypeEthernetLED(GPIO_PORTF_AHB_BASE, GPIO_PIN_0 | GPIO_PIN_4);

  /* PJ0 and J1 are used for user buttons */
  GPIOPinTypeGPIOInput(GPIO_PORTJ_AHB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  GPIOPinWrite(GPIO_PORTJ_AHB_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0);

  /* PN0 and PN1 are used for USER LEDs. */
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_12MA,
                   GPIO_PIN_TYPE_STD);

  /* Default the LEDs to OFF. */
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0);

  /* systick config */
  MAP_SysTickPeriodSet(g_ui32SysClock / SYSTICKHZ);
  MAP_SysTickEnable();
  MAP_SysTickIntEnable();

  EMacInit();

  /* interrupt prio */
  MAP_IntPrioritySet(INT_EMAC0, ETHERNET_INT_PRIORITY);
  MAP_IntPrioritySet(FAULT_SYSTICK, SYSTICK_INT_PRIORITY);

  return 0;
}
