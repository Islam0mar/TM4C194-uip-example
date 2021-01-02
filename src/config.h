/*
 * config.h
 *
 *  Created on: Jan 2, 2021
 *      Author: islam
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#include "uip-conf.h"
#define BUILD_BUG_OR_ZERO(e) (sizeof(struct { int : -!!(e); }))
/* def. for pin_map */
#define PART_TM4C1294NCPDT
/* for ASSERT and logging */
#define DEBUG
#define LOGGING

#endif /* SRC_CONFIG_H_ */
