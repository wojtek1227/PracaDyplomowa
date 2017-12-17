/*
 * uart.c
 *
 *  Created on: 16 gru 2017
 *      Author: caput
 */
#include "uart.h"

static void isr_uart()
{
	static uint8_t i;
	volatile uint8_t data;
	static uint8_t a, s, d, f, h, z, x, c, v, b,n;
	static uint32_t g;
	if(IORD_ALTERA_AVALON_UART_STATUS(UART_BASE) & ALTERA_AVALON_UART_STATUS_RRDY_MSK)
	{
		data = IORD_ALTERA_AVALON_UART_RXDATA(UART_BASE);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, IORD_ALTERA_AVALON_PIO_DATA(PIO_0_BASE) ^ (1<<3));
		switch (data) {
			case 'a':
				*((uint8_t*)VRAM_BASE) ^= (1 << a);
				a++;
				a%=8;
				break;
			case 's':
				*((uint8_t*)VRAM_BASE + 1) ^= (1 << s);
				s++;
				s%=8;
				break;
			case 'd':
				*((uint8_t*)VRAM_BASE + 2) ^= (1 << d);
				d++;
				d%=8;
				break;
			case 'f':
				*((uint8_t*)VRAM_BASE + 3) ^= (1 << f);
				f++;
				f%=8;
				break;
			case 'g':
				*((uint32_t*)VRAM_BASE) ^= (1 << g);
				g++;
				g%=32;
				break;
			case 'h':
				*((uint32_t*)VRAM_BASE + h) = 0;
				h++;
				h%=2;
				break;
			case 'z':
				*((uint8_t*)VRAM_BASE+80 +4) ^= (1 << z);
				z++;
				z%=8;
				break;
			case 'x':
				*((uint8_t*)VRAM_BASE + 1+80+4) ^= (1 << x);
				x++;
				x%=8;
				break;
			case 'c':
				*((uint8_t*)VRAM_BASE + 2+80+4) ^= (1 << c);
				c++;
				c%=8;
				break;
			case 'v':
				*((uint8_t*)VRAM_BASE + 3+80+4) ^= (1 << v);
				v++;
				v%=8;
				break;
			case 'b':
				*((uint32_t*)VRAM_BASE+20+1) ^= (1 << b);
				b++;
				b%=32;
				break;
			case 'n':
				*((uint32_t*)VRAM_BASE + n +20+1) = 0;
				n++;
				n%=2;
				break;
			default:
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


