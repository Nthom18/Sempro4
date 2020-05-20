/*****************************************************************************
* University of Southern Denmark
*
* MODULENAME.: gpio.c
*
* PROJECT....: Semester project F20
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 200403  FBS   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "gpio.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_gpio(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : GPIO setup.
******************************************************************************/
{
  int dummy;

  // Enable the GPIO port that is used for the on-board LED.
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOA | SYSCTL_RCGC2_GPIOF | SYSCTL_RCGC2_GPIOD | SYSCTL_RCGC2_GPIOC;

  // Do a dummy read to insert a few cycles after enabling the peripheral.
  dummy = SYSCTL_RCGC2_R;

  // Set the direction as output
  GPIO_PORTA_DIR_R &= 0x1F;   //Set PA7 to PA5 as input
  GPIO_PORTF_DIR_R = 0x0E;    //(PF1, PF2 and PF3).
  GPIO_PORTD_DIR_R = 0x4C;    // Set the direction as output (PD6). + PD2 & PD3 (LCD)
  GPIO_PORTC_DIR_R = 0xF0;    // Set the direction as output (PC4-PC7). (LCD)

  // Enable the GPIO pins for digital function (PF0, PF1, PF2, PF3, PF4).
  GPIO_PORTA_DEN_R = 0xE0;         //enable digital operation of PA7 to PA5
  GPIO_PORTF_DEN_R = 0x1F;
  GPIO_PORTD_DEN_R = 0x4C;    // Enable the GPIO pins for digital function (PD6).
  GPIO_PORTC_DEN_R = 0xF0;    // Set the direction as output (PC4-PC7). (LCD)

  // Enable internal pull-up (PF0 and PF4).
  GPIO_PORTF_PUR_R = 0x11;
}

/****************************** End Of Module *******************************/