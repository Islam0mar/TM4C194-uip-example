/*
 * logger.c
 *
 *  Created on: Dec 31, 2020
 *      Author: islam
 */

#include "logger.h"

#include <stdint.h>

char* GetLogger() {
  static char* log[LOGGER_SIZE];
  return log;
}

void LogInfo(char* string_ptr) {
  static uint16_t index = 0;
  char* log = GetLogger();
  log[index++] = string_ptr;
  index %= LOGGER_SIZE;
}
