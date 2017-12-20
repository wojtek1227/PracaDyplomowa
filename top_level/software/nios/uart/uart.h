/*
 * uart.h
 *
 *  Created on: 16 gru 2017
 *      Author: caput
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#include "system.h"
#include "sys/alt_irq.h"

#include "altera_avalon_uart_regs.h"
#include "altera_avalon_pio_regs.h"


void init_uart(void);
void uart_send_char(uint8_t data);


#endif /* UART_H_ */
