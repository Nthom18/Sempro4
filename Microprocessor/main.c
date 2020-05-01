/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: main.c
 *
 * PROJECT....: EMP
 *
 * DESCRIPTION: Templatev1
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 150215  MoH   Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "spi.h"
#include "uart0.h"
/*****************************    Defines    *******************************/

#define DATA_SIZE 	8
#define CLK_PSCL	2
#define CLK_PH 		0
#define CLK_PO 		0

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

extern volatile INT16S ticks;

/*****************************   Functions   *******************************/

int main(void) {
	INT16U timer = 0;

	INT8U send_data;
	INT8U receive_data;
	INT8U count = 0;

	//LEDS
	SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_DEN_R = 0x1E;
	GPIO_PORTF_PUR_R = 0x10;

	//Initialize SPI
	spi_init(DATA_SIZE, CLK_PSCL, CLK_PH, CLK_PO);

	init_systick();
	enable_global_int();
	uart0_init(9600, 8, 1, 'n');
	uart0_putca(">> UART RDY");
	uart0_putc('\n');

	while (1) {
		while (!ticks) {
		};

		// The following will be executed every 5mS
		ticks--;


		if (uart0_rx_rdy()) {
			send_data = uart0_getc() - 48;

			//Send test
			//send_byte(send_data);
			//send_byte(count);
			//count++;
			//uart0_putc('\n');

			//Receive test
			receive_data = sr_byte(send_data) + 48;
			uart0_putc('\n');
			uart0_putca(">> ");
			uart0_putc(receive_data);
			uart0_putc('\n');
		}
//		timer++;
//		if (timer == 1000) {
//
//			//Set LED
//			GPIO_PORTF_DATA_R ^= 0x08;
//
//			//timer = 0;
//			send_byte(0b10101010);
//			//send_str("Test");
//
//			//Clear LED
//			//GPIO_PORTF_DATA_R &= 0x00;
//		}
//		else if (timer >= 2000) {
//
//			//Set LED
//			GPIO_PORTF_DATA_R ^= 0x08;
//
//			send_byte(0b01010101);
//
//			timer = 0;
//		}

	}
	return (0);
}

/****************************** End Of Module *******************************/


// 64 & 3
// 128 & 64
// 1 & 2
