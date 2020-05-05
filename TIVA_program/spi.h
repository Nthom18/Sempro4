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
* Date    Id          Change
* YYMMDD
* --------------------
* 200306  CH          Module created.
* 200505  CSN & FBS   SPI task created.
*
*****************************************************************************/

#ifndef _SPI_H
  #define _SPI_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/********************** External declaration of Variables ******************/

/*****************************   Constants   *******************************/

/*************************  Function interfaces ****************************/


void spi_task();
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : SPI task to be run in RTOS,
******************************************************************************/



extern void spi_init(INT8U, INT8U, BOOLEAN, BOOLEAN);
/*****************************************************************************
*   Input    :  Number of data-bits used:   4-16
*               Clock prescaler:            1-255
*               Clock phase:                0-1
*               Clock polarity:             0-1
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

extern INT16U sr_word(INT16U);
/*****************************************************************************
*   Input    : 16 bits of data: 0x0000 to 0xFFFF
*   Output   : 16 bits of data: 0x0000 to 0xFFFF
*   Function : Transmits/Receives 16 bits of data via SPI. Returns when data transfer is complete.
******************************************************************************/


extern INT16U FPGA_update(INT16U, INT16U);
/*****************************************************************************
*   Input    : 2x8 bits of PWM values. First 8 are for motor1, last 8 are for motor2.
*   Output   : 16 bits of data: 0x0000 to 0xFFFF (Upper 8 bits are relative position change of motor1 since last transfer, lower 8 for motor2)
*   Function : Transmits/Receives 16 bits of data via SPI. Returns when data transfer is complete.
******************************************************************************/


void send_str();

/****************************** End Of Module *******************************/
#endif

