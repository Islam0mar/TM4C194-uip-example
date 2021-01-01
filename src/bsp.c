/*
 * bsp.c
 *
 *  Created on: Dec 31, 2020
 *      Author: islam
 */

#include "bsp.h"

#include <stdbool.h>
#include <stdint.h>

#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/flash.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"


void __error__(char *pcFilename, uint32_t ui32Line) {
  /* NVIC_SystemReset(); /\* reset the system *\/ */
  while (1)
    ;
}

void IrqOn() { IntMasterEnable(); }
void IrqOff() { IntMasterDisable(); }
void BspInit() {
  /*  Run from the PLL at 120 MHz.*/
    extern uint32_t g_ui32SysClock;

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
}

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide
an implementation of vApplicationGetIdleTaskMemory() to provide the memory that
is used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) {
  /* If the buffers to be provided to the Idle task are declared inside this
  function then they must be declared static – otherwise they will be allocated
  on the stack and so not exists after this function exits. */
  static StaticTask_t xIdleTaskTCB;
  static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

  /* Pass out a pointer to the StaticTask_t structure in which the Idle task’s
  state will be stored. */
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

  /* Pass out the array that will be used as the Idle task’s stack. */
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;

  /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
  Note that, as the array is necessarily of type StackType_t,
  configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*———————————————————–*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so
the application must provide an implementation of
vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize) {
  /* If the buffers to be provided to the Timer task are declared inside this
  function then they must be declared static – otherwise they will be allocated
  on the stack and so not exists after this function exits. */
  static StaticTask_t xTimerTaskTCB;
  static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

  /* Pass out a pointer to the StaticTask_t structure in which the Timer
  task’s state will be stored. */
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

  /* Pass out the array that will be used as the Timer task’s stack. */
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;

  /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
  Note that, as the array is necessarily of type StackType_t,
  configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
