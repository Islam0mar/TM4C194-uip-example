/*
 * bsp.h
 *
 *  Created on: Dec 31, 2020
 *      Author: islam
 */
#ifndef HAL_BSP_H_INCLUDED
#define HAL_BSP_H_INCLUDED

#include <stdint.h>
#include "debug.h"
#include "tm4c1294ncpdt.h"

//*****************************************************************************
//
// Defines for setting up the system clock.
//
//*****************************************************************************
#define SYSTICKHZ               100
#define SYSTICKMS               (1000 / SYSTICKHZ)

#define HSE_VALUE 8000000U
#define HSI_VALUE 8000000U
/*!< Vector Table base offset field. This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET 0x00000000U

extern uint32_t SystemCoreClock; /*!< System Clock Frequency (Core Clock) */
extern const uint8_t AHBPrescTable[16]; /*!< AHB prescalers table values */
extern const uint8_t APBPrescTable[8];  /*!< APB prescalers table values */
extern int vector_table[];

void SystemClock_Config();
void GPIO_Init();
void BspInit();
void IrqOn();
void IrqOff();
void SystemInit(void);
void SystemCoreClockUpdate(void);

#endif /* HAL_BSP_H_INCLUDED */
