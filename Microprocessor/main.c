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

#define DATA_SIZE 	16
#define CLK_PSCL	2
#define CLK_PH 		0
#define CLK_PO 		0

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

extern volatile INT16S ticks;

/*****************************   Functions   *******************************/

int main(void) {

	INT8U PWM1 = 0b01000000;
	INT8U PWM2 = 0b11000000;
	INT16U receive_data;
	INT8S motor1_angle, motor2_angle;

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
			uart0_getc();

			uart0_putc('\n');

			receive_data = FPGA_update(PWM1, PWM2);
			motor2_angle = (receive_data & 0x00FF);
			motor1_angle = (receive_data >> 8) & 0x00FF;

			uart0_putca(">> Motor1 relative angle: ");
			uart0_putnumber_s(motor1_angle);
			uart0_putc('\n');
			uart0_putca(">> Motor2 relative angle: ");
			uart0_putnumber_s(motor2_angle);
			uart0_putc('\n');
		}
	}
	return (0);
}

/****************************** End Of Module *******************************/


// 64 & 3
// 128 & 64
// 1 & 2
