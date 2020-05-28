/*****************************************************************************
* University of Southern Denmark
* RB-PRO4 - Group 4
* Semesterproject in control and regulation of robotic systems
* Module: joystick.h
* Created 03/04/2020
*****************************************************************************/
#ifndef JOYSTICK_H_
#define JOYSTICK_H_

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

void init_adc(void);
void adc_get_samples(int* x, int* y);
void joystick_calibrate(void);
void joystick_task(void);

/****************************** End Of Module *******************************/
#endif /* JOYSTICK_H_ */
