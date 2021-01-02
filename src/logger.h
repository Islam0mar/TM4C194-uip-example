/*
 * logger.h
 *
 *  Created on: Jan 2, 2021
 *      Author: islam
 */

#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_

#ifndef LOGGING
#define LOG_INFO(x)
#define LOGGER_SIZE 1
#else
#define LOG_INFO(x) LogInfo(x);
#define LOGGER_SIZE 1024
void LogInfo(char* string_ptr);
char* GetLogger();
#endif

#endif /* SRC_LOGGER_H_ */
