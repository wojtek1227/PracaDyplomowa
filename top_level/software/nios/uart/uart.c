/*
 * uart.c
 *
 *  Created on: 16 gru 2017
 *      Author: caput
 */
#include "uart.h"

static void isr_uart()
{
	volatile alt_8 data;
	if(IORD_ALTERA_AVALON_UART_STATUS(UART_BASE) & ALTERA_AVALON_UART_STATUS_RRDY_MSK)
	{
		data = IORD_ALTERA_AVALON_UART_RXDATA(UART_BASE);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, IORD_ALTERA_AVALON_PIO_DATA(PIO_0_BASE) ^ (1<<3));
	}
}

void init_uart(void)
{
	IOWR_ALTERA_AVALON_UART_CONTROL(UART_BASE, ALTERA_AVALON_UART_CONTROL_RRDY_MSK);
	alt_ic_isr_register(UART_IRQ_INTERRUPT_CONTROLLER_ID, UART_IRQ, isr_uart, NULL, 0);
	alt_ic_irq_enable(UART_IRQ_INTERRUPT_CONTROLLER_ID, UART_IRQ);
}


