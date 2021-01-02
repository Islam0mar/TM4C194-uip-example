/*
 * bsp.h
 *
 *  Created on: Dec 31, 2020
 *      Author: islam
 */
#ifndef HAL_BSP_H_INCLUDED
#define HAL_BSP_H_INCLUDED

#include <stdint.h>

#include "clock-arch.h"
#include "driverlib/debug.h"
#include "uip-conf.h"

/* systick freq */
#ifndef CLOCK_CONF_SECOND
#define CLOCK_CONF_SECOND 1000
typedef uint32_t clock_time_t;
#endif
#define SYSTICKHZ (uint32_t) CLOCK_CONF_SECOND
#define SYSTICKMS (uint32_t)(1000 / SYSTICKHZ)

/* interrupt prio */
#define SYSTICK_INT_PRIORITY (uint8_t)0x80
#define ETHERNET_INT_PRIORITY (uint8_t)0xc0

/*!< System Clock Frequency (Core Clock) */
extern uint32_t g_ui32SysClock;

/* global flags for events*/
extern uint32_t g_ui32Flags;

uint8_t BspInit();
uint8_t *BspGetMacAddress();
void BspIrqOn();
void BspIrqOff();
uint32_t BspGetMSecTime();
int32_t BspPacketTransmit(uint8_t *pui8Buf, int32_t i32BufLen);
int32_t BspPacketReceive(uint8_t *pui8Buf, int32_t i32BufLen);

#endif /* HAL_BSP_H_INCLUDED */
