/*
 * log.c
 *
 *  Created on: Dec 31, 2020
 *      Author: islam
 */

#include <stdint.h>

#define LOG_SIZE 1024

void UpdateStatus(char* string_ptr) {
  static char* log[LOG_SIZE];
  static uint16_t index = 0;
  log[index++] = string_ptr;
  index %= LOG_SIZE;
}
