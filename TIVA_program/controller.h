/*****************************************************************************
* University of Southern Denmark
* RB-PRO4 - Group 4
* Semesterproject in control and regulation of robotic systems
* Module: controller.h
* Created 12/05/2020
*****************************************************************************/

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void pid_controller_pan(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Implementation of the designed controller for the pan frame.
******************************************************************************/

void pid_controller_tilt(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Implementation of the designed controller for the tilt frame.
******************************************************************************/


/****************************** End Of Module *******************************/
#endif /* CONTROLLER_H_ */
