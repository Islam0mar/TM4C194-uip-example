/**
 *   \file main.c
 *   \brief A Documented file.
 *
 *  Copyright (c) 2020 Islam Omar (io1131@fayoum.edu.eg)
 *
 */

#include "bsp.h"
/* FreeRTOS kernel includes. */
#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "driverlib/flash.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "event_groups.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "queue.h"
#include "task.h"
#include "timers.h"

//*****************************************************************************
//
// Interrupt priority definitions.  The top 3 bits of these values are
// significant with lower values indicating higher priority interrupts.
//
//*****************************************************************************
#define SYSTICK_INT_PRIORITY 0x80
#define ETHERNET_INT_PRIORITY 0xC0

//*****************************************************************************
//
// The current IP address.
//
//*****************************************************************************
uint32_t g_ui32IPAddress;

//*****************************************************************************
//
// The system clock frequency.
//
//*****************************************************************************
uint32_t g_ui32SysClock;

//*****************************************************************************
//
// Volatile global flag to manage LED blinking, since it is used in interrupt
// and main application.  The LED blinks at the rate of SYSTICKHZ.
//
//*****************************************************************************
volatile bool g_bLED;

//*****************************************************************************
//
// Required by lwIP library to support any host-related timer functions.
//
//*****************************************************************************
void lwIPHostTimerHandler(void) {
  uint32_t ui32NewIPAddress;

  //
  // Get the current IP address.
  //
  //  ui32NewIPAddress = lwIPLocalIPAddrGet();

  //
  // See if the IP address has changed.
  //
  if (ui32NewIPAddress != g_ui32IPAddress) {
    //
    // See if there is an IP address assigned.
    //
    if (ui32NewIPAddress == 0xffffffff) {
      //
      // Indicate that there is no link.
      //
    } else if (ui32NewIPAddress == 0) {
      //
      // There is no IP address, so indicate that the DHCP process is
      // running.
      //
    } else {
      //
      // Display the new IP address.
      //
      //      UARTprintf("IP Address: ");
      //      DisplayIPAddress(ui32NewIPAddress);
    }

    //
    // Save the new IP address.
    //
    g_ui32IPAddress = ui32NewIPAddress;
  }

  //
  // If there is not an IP address.
  //
  if ((ui32NewIPAddress == 0) || (ui32NewIPAddress == 0xffffffff)) {
    //
    // Do nothing and keep waiting.
    //
  }
}

//*****************************************************************************
//
// The interrupt handler for the SysTick interrupt.
//
//*****************************************************************************
void SysTickIntHandler(void) {
  //
  // Call the lwIP timer handler.
  //
  //    lwIPTimer(SYSTICKMS);

  //
  // Tell the application to change the state of the LED (in other words
  // blink).
  //
  g_bLED = true;
}

void vTask1(void* pvParams);
void vTask2(void* pvParams);
/* Dimensions the buffer that the task being created will use as its stack.
   NOTE:  This is the number of words the stack will hold, not the number of
   bytes.  For example, if each stack item is 32-bits, and this is set to 100,
   then 400 bytes (100 * 32-bits) will be allocated. */
#define STACK_SIZE 200U

/* Structure that will hold the TCB of the task being created. */
StaticTask_t xTask1Buffer;
StaticTask_t xTask2Buffer;

/* Buffer that the task being created will use as its stack.  Note this is
an array of StackType_t variables.  The size of StackType_t is dependent on
the RTOS port. */
StackType_t xStackTask1[STACK_SIZE];
StackType_t xStackTask2[STACK_SIZE];

//*****************************************************************************
//
// This example demonstrates the use of the Ethernet Controller.
//
//*****************************************************************************
int main(void) {
  uint32_t ui32User0, ui32User1;
  uint8_t pui8MACArray[8];

  /* TODO: HW failure */
  BspInit();

  /* Create the task without using any dynamic memory allocation. */
  xTaskCreateStatic(
      vTask1,         /* Function that implements the task. */
      "T1",           /* Text name for the task. */
      STACK_SIZE,     /* Number of indexes in the xStack array. */
      (void*)1,       /* Parameter passed into the task. */
      5,              /* Priority at which the task is created. */
      xStackTask1,    /* Array to use as the task's stack. */
      &xTask1Buffer); /* Variable to hold the task's data structure. */

  xTaskCreateStatic(
      vTask2,         /* Function that implements the task. */
      "T2",           /* Text name for the task. */
      STACK_SIZE,     /* Number of indexes in the xStack array. */
      (void*)1,       /* Parameter passed into the task. */
      6,              /* Priority at which the task is created. */
      xStackTask2,    /* Array to use as the task's stack. */
      &xTask2Buffer); /* Variable to hold the task's data structure. */

  //
  // Configure SysTick for a periodic interrupt.
  //
  MAP_SysTickPeriodSet(g_ui32SysClock / SYSTICKHZ);
  MAP_SysTickEnable();
  MAP_SysTickIntEnable();

  //
  // Configure the hardware MAC address for Ethernet Controller filtering of
  // incoming packets.  The MAC address will be stored in the non-volatile
  // USER0 and USER1 registers.
  //
  MAP_FlashUserGet(&ui32User0, &ui32User1);
  if ((ui32User0 == 0xffffffff) || (ui32User1 == 0xffffffff)) {
    //
    // We should never get here.  This is an error if the MAC address has
    // not been programmed into the device.  Exit the program.
    // Let the user know there is no MAC address
    //
    while (1) {
    }
  }

  //
  // Convert the 24/24 split MAC address from NV ram into a 32/16 split MAC
  // address needed to program the hardware registers, then program the MAC
  // address into the Ethernet Controller registers.
  //
  pui8MACArray[0] = ((ui32User0 >> 0) & 0xff);
  pui8MACArray[1] = ((ui32User0 >> 8) & 0xff);
  pui8MACArray[2] = ((ui32User0 >> 16) & 0xff);
  pui8MACArray[3] = ((ui32User1 >> 0) & 0xff);
  pui8MACArray[4] = ((ui32User1 >> 8) & 0xff);
  pui8MACArray[5] = ((ui32User1 >> 16) & 0xff);

  //
  // Initialize the lwIP library, using DHCP.
  //
  //  lwIPInit(g_ui32SysClock, pui8MACArray, 0, 0, 0, IPADDR_USE_DHCP);

  //
  // Set the interrupt priorities.  We set the SysTick interrupt to a higher
  // priority than the Ethernet interrupt to ensure that the file system
  // tick is processed if SysTick occurs while the Ethernet handler is being
  // processed.  This is very likely since all the TCP/IP and HTTP work is
  // done in the context of the Ethernet interrupt.
  //
  MAP_IntPrioritySet(INT_EMAC0, ETHERNET_INT_PRIORITY);
  MAP_IntPrioritySet(FAULT_SYSTICK, SYSTICK_INT_PRIORITY);

  vTaskStartScheduler();
}

void vTask1(void* pvParams) {
  /* Block for 500ms. */
  const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

  for (;;) {
    //              vToggleLED();
    vTaskDelay(xDelay);
  }
}

void vTask2(void* pvParams) {
  /* Block for 500ms. */
  const TickType_t xDelay = 600 / portTICK_PERIOD_MS;

  for (;;) {
    vTaskDelay(xDelay);
  }
}
