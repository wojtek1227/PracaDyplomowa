/*
 * uart.c
 *
 *  Created on: 16 gru 2017
 *      Author: caput
 */
#include "uart.h"

extern volatile uint8_t shoot_button_uart;
extern volatile uint8_t left_button_uart;
extern volatile uint8_t right_button_uart;

static void isr_uart()
{
	if(IORD_ALTERA_AVALON_UART_STATUS(UART_BASE) & ALTERA_AVALON_UART_STATUS_RRDY_MSK)
	{
		switch (IORD_ALTERA_AVALON_UART_RXDATA(UART_BASE)) {
			case 'a':
				left_button_uart = 1;
				break;
			case 'c':
				shoot_button_uart = 0;
				break;
			case 'd':
				right_button_uart = 1;
				break;
			case ' ':
				shoot_button_uart = 1;
				break;
			default:
				left_button_uart = 0;
				right_button_uart = 0;
				break;
		}
	}
}

void init_uart(void)
{
	IOWR_ALTERA_AVALON_UART_CONTROL(UART_BASE, ALTERA_AVALON_UART_CONTROL_RRDY_MSK);
	alt_ic_isr_register(UART_IRQ_INTERRUPT_CONTROLLER_ID, UART_IRQ, isr_uart, NULL, 0);
	alt_ic_irq_enable(UART_IRQ_INTERRUPT_CONTROLLER_ID, UART_IRQ);
}

void uart_send_char(uint8_t data)
{
	while(!(IORD_ALTERA_AVALON_UART_STATUS(UART_BASE) & ALTERA_AVALON_UART_STATUS_TRDY_MSK)){}
	IOWR_ALTERA_AVALON_UART_TXDATA(UART_BASE, data);
}

