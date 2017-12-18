/*
 * timers.c
 *
 *  Created on: 16 gru 2017
 *      Author: caput
 */
#include "timers.h"


volatile static uint32_t time_ms;

volatile uint8_t left_button;
volatile uint8_t right_button;
volatile uint8_t shoot_button;

static void isr_timer0()
{
	time_ms++;
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, ALTERA_AVALON_TIMER_STATUS_TO_MSK);

}

static void isr_timer1()
{

//	shoot_button = !(IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE) & (1 << 0));
	shoot_button = (IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE) & (1 << 0)) ? 0 : 1;
	if (!(IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE) & (1 << 2)))
	{

	}
	left_button = !(IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE) & (1 << 2));
	right_button = !(IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE) & (1 << 1));
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_1_BASE, ALTERA_AVALON_TIMER_STATUS_TO_MSK);
}

void init_timers(void)
{
	//Timer 0 init
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE,
									ALTERA_AVALON_TIMER_CONTROL_START_MSK |
									ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
									ALTERA_AVALON_TIMER_CONTROL_ITO_MSK);
	alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, isr_timer0, NULL, 0);
	alt_ic_irq_enable(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ);
	//Timer 1 init
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_1_BASE,
									ALTERA_AVALON_TIMER_CONTROL_START_MSK |
									ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
									ALTERA_AVALON_TIMER_CONTROL_ITO_MSK);
	alt_ic_isr_register(TIMER_1_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_1_IRQ, isr_timer1, NULL, 0);
	alt_ic_irq_enable(TIMER_1_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_1_IRQ);
}

uint32_t get_time(void)
{
	return time_ms;
}
