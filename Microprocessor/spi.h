/*****************************************************************************
* University of Southern Denmark
*
*
* MODULENAME.: spi.h
*
* PROJECT....: EMP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 200306  CH    Module created.
*
*****************************************************************************/

#ifndef _SPI_H
  #define _SPI_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/********************** External declaration of Variables ******************/

/*****************************   Constants   *******************************/

/*************************  Function interfaces ****************************/

extern void spi_init(INT8U, INT8U, BOOLEAN, BOOLEAN);
/*****************************************************************************
*   Input    :	Number of data-bits used: 	4-16
*   			Clock prescaler: 			1-255
*   			Clock phase: 				0-1
*   			Clock polarity: 			0-1
*   Output   : -
*   Function : Initialization of SPI for pin PB4 (SCK), PB5 (SS), PB6 (MISO), and PB7 (MOSI).
******************************************************************************/

extern void send_byte(INT8U);
/*****************************************************************************
*   Input    : Byte of data: 0-255
*   Output   : -
*   Function : Transmits byte of data via SPI. Returns when data transfer is complete.
******************************************************************************/

extern INT8U receive_byte();
/*****************************************************************************
*   Input    : -
*   Output   : Byte of data: 0-255
*   Function : Receives byte of data via SPI. Returns when data transfer is complete.
******************************************************************************/

extern INT8U sr_byte(INT8U);
/*****************************************************************************
*   Input    : Byte of data: 0-255
*   Output   : Byte of data: 0-255
*   Function : Transmits/Receives byte of data via SPI. Returns when data transfer is complete.
******************************************************************************/

extern void send_pwm(INT8U, INT8U);
/*****************************************************************************
*   Input    : Byte of data: 0-255, motor number (1 or 2).
*   Output   :
*   Function : Transmits pwm instruction
******************************************************************************/

extern INT8U receive_angle(INT8U);
/*****************************************************************************
*   Input    : Motor number (1 or 2).
*   Output   :
*   Function : Receives angular position of chosen motor.
******************************************************************************/

void send_str();

/****************************** End Of Module *******************************/
#endif

