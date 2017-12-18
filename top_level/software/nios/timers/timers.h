/*
 * timer.h
 *
 *  Created on: 16 gru 2017
 *      Author: caput
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <stdbool.h>

#include "system.h"
#include "sys/alt_irq.h"

#include "altera_avalon_timer_regs.h"
#include "altera_avalon_pio_regs.h"

void init_timers(void);
uint32_t get_time(void);

#endif /* TIMER_H_ */
