/* 
 * "Small Hello World" example. 
 * 
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example 
 * designs. It requires a STDOUT  device in your system's hardware. 
 *
 * The purpose of this example is to demonstrate the smallest possible Hello 
 * World application, using the Nios II HAL library.  The memory footprint
 * of this hosted application is ~332 bytes by default using the standard 
 * reference design.  For a more fully featured Hello World application
 * example, see the example titled "Hello World".
 *
 * The memory footprint of this example has been reduced by making the
 * following changes to the normal "Hello World" example.
 * Check in the Nios II Software Developers Manual for a more complete 
 * description.
 * 
 * In the SW Application project (small_hello_world):
 *
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 * In System Library project (small_hello_world_syslib):
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 *    - Define the preprocessor option ALT_NO_INSTRUCTION_EMULATION 
 *      This removes software exception handling, which means that you cannot 
 *      run code compiled for Nios II cpu with a hardware multiplier on a core 
 *      without a the multiply unit. Check the Nios II Software Developers 
 *      Manual for more details.
 *
 *  - In the System Library page:
 *    - Set Periodic system timer and Timestamp timer to none
 *      This prevents the automatic inclusion of the timer driver.
 *
 *    - Set Max file descriptors to 4
 *      This reduces the size of the file handle pool.
 *
 *    - Check Main function does not exit
 *    - Uncheck Clean exit (flush buffers)
 *      This removes the unneeded call to exit when main returns, since it
 *      won't.
 *
 *    - Check Don't use C++
 *      This builds without the C++ support code.
 *
 *    - Check Small C library
 *      This uses a reduced functionality C library, which lacks  
 *      support for buffering, file IO, floating point and getch(), etc. 
 *      Check the Nios II Software Developers Manual for a complete list.
 *
 *    - Check Reduced device drivers
 *      This uses reduced functionality drivers if they're available. For the
 *      standard design this means you get polled UART and JTAG UART drivers,
 *      no support for the LCD driver and you lose the ability to program 
 *      CFI compliant flash devices.
 *
 *    - Check Access device drivers directly
 *      This bypasses the device file system to access device drivers directly.
 *      This eliminates the space required for the device file system services.
 *      It also provides a HAL version of libc services that access the drivers
 *      directly, further reducing space. Only a limited number of libc
 *      functions are available in this configuration.
 *
 *    - Use ALT versions of stdio routines:
 *
 *           Function                  Description
 *        ===============  =====================================
 *        alt_printf       Only supports %s, %x, and %c ( < 1 Kbyte)
 *        alt_putstr       Smaller overhead than puts with direct drivers
 *                         Note this function doesn't add a newline.
 *        alt_putchar      Smaller overhead than putchar with direct drivers
 *        alt_getchar      Smaller overhead than getchar with direct drivers
 *
 */

#include "system.h"
#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_uart_regs.h"

#include "buttons/buttons.h"

void init_system(void);

static void isr_timer0()
{
	static int counter = 1;
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, IORD_ALTERA_AVALON_PIO_DATA(PIO_0_BASE) ^ (1<<0));
//	IOWR_ALTERA_AVALON_PIO_SET_BITS(PIO_0_BASE,counter);
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);
	counter^= (int)0x01;
//	counter++;
//	if(IORD_ALTERA_AVALON_UART_STATUS(UART_BASE) & ALTERA_AVALON_UART_STATUS_TRDY_MSK)
//	{
//		IOWR_ALTERA_AVALON_UART_TXDATA(UART_BASE,0x66);
//	}
}

static void isr_uart()
{
	static int counter = 2;
	volatile alt_8 data;
	if(IORD_ALTERA_AVALON_UART_STATUS(UART_BASE) & ALTERA_AVALON_UART_STATUS_RRDY_MSK)
	{
		data = IORD_ALTERA_AVALON_UART_RXDATA(UART_BASE);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, IORD_ALTERA_AVALON_PIO_DATA(PIO_0_BASE) ^ (1<<1));
	}
}


int main()
{ 
	init_system();
	/* Event loop never exits. */
	while (1)
	{

	}

	return 0;
}

void init_system(void)
{
	//Timer initialization
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE,
									ALTERA_AVALON_TIMER_CONTROL_START_MSK |
									ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
									ALTERA_AVALON_TIMER_CONTROL_ITO_MSK);
	alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, isr_timer0, NULL, 0);
	alt_ic_irq_enable(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ);

	//UART initialization
	IOWR_ALTERA_AVALON_UART_CONTROL(UART_BASE, ALTERA_AVALON_UART_CONTROL_RRDY_MSK);
	alt_ic_isr_register(UART_IRQ_INTERRUPT_CONTROLLER_ID, UART_IRQ, isr_uart, NULL, 0);
	alt_ic_irq_enable(UART_IRQ_INTERRUPT_CONTROLLER_ID, UART_IRQ);
}
