/*
 * ring-buffer.c
 *
 *  Created on: Jan 2, 2021
 *      Author: islam
 */
#include "ring-buffer.h"

#include "driverlib/debug.h"

static uint8_t *ring_buffer;
static uint16_t ring_buffer_insert_index;
static uint16_t ring_buffer_get_index;

#define INSERT_SIZE(x)                                         \
  do {                                                         \
    *((uint16_t *)&ring_buffer[ring_buffer_insert_index]) = x; \
    ring_buffer_insert_index += 2;                             \
  } while (0)

void RingBufferInsert(RingBuffer_t *pb, uint8_t *data, uint16_t size) {
  ASSERT(RING_BUFFER_SIZE >= size + 2);
  ring_buffer = pb->ring_buffer;
  ring_buffer_insert_index = pb->ring_buffer_insert_index;
  /* new data will fit in the free region inside the buffer*/
  if (RING_BUFFER_SIZE >= size + ring_buffer_insert_index + 2) {
    INSERT_SIZE(size);
    MemCpy(&ring_buffer[ring_buffer_insert_index], data, size);
    ring_buffer_insert_index += size;
    ring_buffer_insert_index %= RING_BUFFER_SIZE;
  } else {
    /* data would wrap around, so*/
    /* 1. copy the protion of data the would fit inside the buffer */
    if (RING_BUFFER_SIZE - ring_buffer_insert_index > 1) {
      INSERT_SIZE(size);
      MemCpy(&ring_buffer[ring_buffer_insert_index], data,
             RING_BUFFER_SIZE - ring_buffer_insert_index);
      size -= RING_BUFFER_SIZE - ring_buffer_insert_index;
      ring_buffer_insert_index = 0;
    } else {
      ring_buffer_insert_index = 0;
      INSERT_SIZE(size);
    }
    /* 2. copy the remaining data from the beginning of the buffer */
    MemCpy(ring_buffer, data, size);
    ring_buffer_insert_index += size;
    /* update src */
    pb->ring_buffer_insert_index = ring_buffer_insert_index;
  }
}
void RingBufferGet(RingBuffer_t *pb, uint8_t **data, uint16_t *size) {
  ring_buffer = pb->ring_buffer;
  ring_buffer_get_index = pb->ring_buffer_get_index;
  if (ring_buffer_get_index + 2 > RING_BUFFER_SIZE) {
    *size = *((uint16_t *)ring_buffer);
    ring_buffer_get_index = 0;
  } else {
    *size = *((uint16_t *)&ring_buffer[ring_buffer_get_index]);
  }
  ring_buffer_get_index += 2;
  ring_buffer_get_index %= RING_BUFFER_SIZE;
  *data = &ring_buffer[ring_buffer_get_index];
  ring_buffer_get_index += *size;
  ring_buffer_get_index %= RING_BUFFER_SIZE;
  /* update src */
  pb->ring_buffer_get_index = ring_buffer_get_index;
}

void RingBufferIncrement() {}
