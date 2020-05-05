/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: glob_def.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Definements of variable types.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef _GLOB_DEF_H
  #define _GLOB_DEF_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

#define FALSE   0
#define TRUE  !FALSE
#define NULL    ((void *)0)
#define NEGATIVE 0
#define POSITIVE 1
#define LEFT    0
#define RIGHT   1
#define UP      1
#define DOWN    0
#define ON      1
#define OFF     0
#define ONEMINUTE 12000
#define ONESECOND 200

#define DONE    2

////Clock commands
//#define TURN_ON_OFF     '0'
//#define SET_CLOCK       '1'
//#define GET_CLOCK       '2'


////Queues
//#define QUEUE_UART0_ID  1
////#define QUEUE_SPI_TX_ID  2

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/
#endif
