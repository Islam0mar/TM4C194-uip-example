/*
 * states.h
 *
 *  Created on: Jan 2, 2021
 *      Author: islam
 */

#ifndef SRC_STATES_H_
#define SRC_STATES_H_

#include <stdint.h>

#include "ring-buffer.h"
#include "uip-conf.h"

/* This is the state
   of our application, and the memory required for this state is
   allocated together with each TCP connection. One application state
   for each TCP connection. */
typedef enum {
  kEmacReceivedFrame = 1,
  kTimeOut,
  kAbort,
  kAck,
  kEmacTransferredFrame
} Signals_t;

typedef enum {
  kIdle,
  kWaitForAck,
  kAborted,
  kSendingLargeData,
  kDhcpWaitForOffer,
  kDhcpWaitForAck
} MainStates_t;

typedef struct {
  MainStates_t state;
  uint16_t *data_ptr;
  uint16_t data_left_size;
  uint8_t count;
} AppState_t;

#endif /* SRC_STATES_H_ */
