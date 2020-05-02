/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: spi.c
 *
 * PROJECT....: EMP
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 200306  CH    Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include "emp_type.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

extern void spi_init(INT8U DDS, INT8U CPSDVSR, BOOLEAN SPH, BOOLEAN SPO)
/*****************************************************************************
 *   Function : See module specification (.h-file).
 *****************************************************************************/
{
	//Note: further division can be done in the SSI2_CR0_R register if necessary

	//Enable and provide a clock to SSI module 2 in Run mode
	SYSCTL_RCGCSSI_R |= (1 << 2);

	//Enable and provide a clock to GPIO Port B in Run mode
	SYSCTL_RCGCGPIO_R |= (1 << 1);

	//Enable alternate functions for relevant pins
	GPIO_PORTB_AFSEL_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);

	//Enable SSI2Clk for PB4, SSI2Fss for PB5, SSI2Rx for PB6, and SSI2Tx for PB7
	GPIO_PORTB_PCTL_R = 0x22220000;

	//Enable digital function of PB4, PB5, PB6, and PB7
	GPIO_PORTB_DEN_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);

	//Enable pull-up resistors on PB4, PB5, PB6, and PB7
	GPIO_PORTB_PUR_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);

	//Enable master mode
	SSI2_CR1_R = 0x00000000;

	//CPU clock source
	SSI2_CC_R &= 0x0;

	// Load divisor into prescaler register
	SSI2_CPSR_R = CPSDVSR;	//SSInClk = SysClk / (CPSDVSR * (1 + SCR))

	//Set number of data bits
	SSI2_CR0_R |= DDS - 1;

	//Set polarity
	SSI2_CR0_R |= (SPH << 7);
	SSI2_CR0_R |= (SPO << 6);

	//NB!
	//SSI2_CR0_R |= 0x400;

	//Enable SSI
	SSI2_CR1_R |= 0x2;
}

extern void send_byte(INT8U data) {
	//Load FIFO transmit buffer with data
	SSI2_DR_R = (0x00FF & data);

	//wait for transmit to complete
	while ((SSI2_SR_R & 1) == 0)
		;
}

extern INT8U receive_byte() {
	//Correct?
	INT8U data;

	//Load FIFO transmit buffer with data
	SSI2_DR_R = 0x00;

	//wait for receive to complete
	while ((SSI2_SR_R & 1) == 0)
		;

	//Read bit 0 to 7
	data = (SSI2_DR_R & 0x00FF);
	return data;
}

extern INT8U sr_byte(INT8U data_send) {
	INT16U data_receive = 0;

	//Load FIFO transmit buffer with data
	SSI2_DR_R = data_send;

	//wait for send/receive to complete
	while ((SSI2_SR_R & 1) == 0)
		;

	while((SSI2_SR_R & 0x04) == 0);		//wait until data is in the receive FIFO before reading

	//Read bit 0 to 7
	while (SSI2_SR_R & 0x04) {
		data_receive = (SSI2_DR_R & 0x00FF);
	}
	//data_receive = (SSI2_DR_R & 0x00FF);
	return data_receive;
}

extern void send_pwm(INT8U pwm_value, INT8U motor_number) {
	INT8U instruction_data = 0x80;
	if (motor_number == 1)
		instruction_data = instruction_data | 0x02;
	else if (motor_number == 2)
		instruction_data = instruction_data | 0x03;
	else
		return;

	sr_byte(instruction_data);
	sr_byte(pwm_value);
}

extern INT8U receive_angle(INT8U motor_number) {
	INT8U instruction_data = 0x00;
	INT8U angle;
	if (motor_number == 1)
			instruction_data = instruction_data | 0x00;
		else if (motor_number == 2)
			instruction_data = instruction_data | 0x01;
		else
			return -1; //Temporary return if invalid motor number

	sr_byte(instruction_data);
	angle = sr_byte(0x00);	//dummy transfer to receive data;

	return angle;
}

void send_str(char *buffer) { //function for sending each byte of string one by one
	while (*buffer != 0) {
		send_byte(*buffer);
		buffer++;
	}
}

void spi_handler() {
	//Catch
	while (1)
		;
}

/****************************** End Of Module *******************************/

