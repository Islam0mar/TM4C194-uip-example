/*
 * ring-buffer.h
 *
 *  Created on: Jan 2, 2021
 *      Author: islam
 */

#ifndef SRC_RING_BUFFER_H_
#define SRC_RING_BUFFER_H_
#include <stdint.h>

#define RING_BUFFER_SIZE (uint16_t)(1518 * 4 + 4)

/* https://www.embedded.com/optimizing-memcpy-improves-speed/ */
static inline void *MemCpy(void *dst, const void *src, uint16_t len) {
  static uint32_t *itr_dst;
  static const uint32_t *itr_src;
  static uint16_t i;
  itr_dst = (uint32_t *)dst;
  itr_src = (const uint32_t *)src;

  if (!(((uint32_t)src) & 0xFFFFFFFC) && !(((uint32_t)dst) & 0xFFFFFFFC)) {
    while (len >= 4) {
      *itr_dst++ = *itr_src++;
      len -= 4;
    }
  }

  /* remaining */
  i = 0;
  while (len-- > 0) {
    *((uint8_t *)((intptr_t)dst + i)) = *((uint8_t *)((intptr_t)src + i));
    i++;
  }

  return (dst);
}
typedef struct RingBuffer_ {
  uint8_t ring_buffer[RING_BUFFER_SIZE];
  uint16_t ring_buffer_insert_index;
  uint16_t ring_buffer_get_index;
} RingBuffer_t;

void RingBufferInsert(RingBuffer_t *, uint8_t *data, uint16_t size);
void RingBufferGet(RingBuffer_t *, uint8_t **data, uint16_t *size);

#endif /* SRC_RING_BUFFER_H_ */
