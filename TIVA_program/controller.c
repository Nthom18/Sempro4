/*****************************************************************************
* University of Southern Denmark
* RB-PRO4 - Group 4
* Semesterproject in control and regulation of robotic systems
* Module: gpio.c
* Created 12/05/2020
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "task_manager.h"
#include "glob_def.h"
#include "conversion.h"
/*****************************    Defines    *******************************/


#define UMAX        12
#define UMIN        -12

#define TICKS_PAN_CW   241.1
#define TICKS_PAN_CCW  264.8
#define TICKS_TILT_CW  251.3
#define TICKS_TILT_CCW 270

#define PWM_LIM_PAN_CW   10
#define PWM_LIM_PAN_CCW   9
#define PWM_LIM_TILT_CW  20
#define PWM_LIM_TILT_CCW 25

#define POS_LIM_PAN_CW  -1.46
#define POS_LIM_PAN_CCW  1.46

//Queues for samples from frame 1 and 2
extern QueueHandle_t placementPanFrameQueue, placementTiltFrameQueue;

//PWM Motor registers
extern INT8U pwm_pan;
extern INT8U pwm_tilt;


extern QueueHandle_t uart0Queue;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

extern FP32 joystick_pan;
extern FP32 joystick_tilt;


/*****************************   Functions   *******************************/
void pid_controller_pan(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : .
******************************************************************************/
{
    FP64 Kp    =      4.3555;        //Proportional gain
    FP64 Ti    =      51.5717;		 //Integral constant
    FP64 Td    =      0.11358;       //Derivative constant
    FP64 Ts    =      0.01;          //Sampling time

    FP64 N     =      28.8267;       //Order of lowpass filter


    FP64 e = 0, w0 = 0, w1 = 0, w2 = 0, u0 = 0;

    INT8U u0_PWM;

    FP64 r = 0;         	//reference

    FP64 Ki = Kp/Ti;        //Integral gain    --set to zero when out of bounds,
    FP64 Kd = Kp*Td;        //Derivative gain

    INT8S y_third_degree;
    FP32  y_rad = 0;

    FP64 a0 = 1;
    FP64 a1 = -4/(Ts*(N+2/Ts));
    FP64 a2 = -(N-2/Ts)/(N+2/Ts);
    FP64 b0 = (Ts*((2*Ki + 2*Kp*N)/Ts + (4*(Kp + Kd*N))/(Ts*Ts) + Ki*N))/(2*(N + 2/Ts));
    FP64 b1 = -(Ts*((8*(Kp + Kd*N))/(Ts*Ts) - 2*Ki*N))/(2*(N + 2/Ts));
    FP64 b2 = (Ts*((4*(Kp + Kd*N))/(Ts*Ts) - (2*Ki + 2*Kp*N)/Ts + Ki*N))/(2*(N + 2/Ts));

    INT8U y_rad_array_TEMP[] = "None  ";
	INT8U *ptr_y_rad_array_TEMP = &y_rad_array_TEMP;

	INT16S y_rad_temp = 0;
	INT8U uart_counter = 0;
	const INT8U ch = 'P';

    while (1)
    {
    	r += joystick_pan;

    	// Limit reference to workspace bounds.
    	if (r < POS_LIM_PAN_CW) {			// Check if r is exceeding lower bound.
    		r = POS_LIM_PAN_CW;				//Limit r to lower bound
    	} else if (r > POS_LIM_PAN_CCW) {   // Check if r is exceeding upper bound.
    		r = POS_LIM_PAN_CCW;			//Limit r to upper bound
    	}

        //Blocks task if queue is empty, otherwise save next value from queue in receivedValue
        xQueueReceive( placementPanFrameQueue, &y_third_degree,  portMAX_DELAY );

        // Temp value for logging position
        y_rad_temp += y_third_degree;

        //Convert from encoder ticks to rad. Take the CW and CCW resolution into account.
        if (y_third_degree > 0) {
        	y_rad += (2*PI/TICKS_PAN_CCW)*y_third_degree;	//Convert from positive ticks to rad.
        } else {
        	y_rad += (2*PI/TICKS_PAN_CW)*y_third_degree;   //Convert from negative ticks to rad.
        }


        // Write value to uart for logging
		uart_counter++;
		if (uart_counter >= 50) {
			uart_counter = 0;
			floatToArray(y_rad, ptr_y_rad_array_TEMP);
			log_data(ch, ptr_y_rad_array_TEMP);
		}

        //update variables,
        w2 = w1;
        w1 = w0;

        e = r - y_rad;          //compute new error

        w0 = e - a1*w1 -a2 * w2;

        u0 = b0 * w0 + b1 * w1 + b2 * w2;      //difference equation,


        //Anti-windup
        if ((u0 > UMAX) || (u0 < UMIN) )
        {
            Ki = 0;
            b0 = (Ts*((2*Ki + 2*Kp*N)/Ts + (4*(Kp + Kd*N))/(Ts*Ts) + Ki*N))/(2*(N + 2/Ts));
            b1 = -(Ts*((8*(Kp + Kd*N))/(Ts*Ts) - 2*Ki*N))/(2*(N + 2/Ts));
            b2 = (Ts*((4*(Kp + Kd*N))/(Ts*Ts) - (2*Ki + 2*Kp*N)/Ts + Ki*N))/(2*(N + 2/Ts));
            u0 = b0*w0 + b1 * w1 + b2 * w2;      //difference equation,
        }


        //saturation
        if (u0 > UMAX)
        {
            u0 = UMAX;          //Limit to PWM range,
        }
        if (u0 < UMIN)
        {
            u0 = UMIN;          //Limit to PWM range,
        }


        //reset values
        Ki = Kp/Ti;        //Integral gain
        b0 = (Ts*((2*Ki + 2*Kp*N)/Ts + (4*(Kp + Kd*N))/(Ts*Ts) + Ki*N))/(2*(N + 2/Ts));
        b1 = -(Ts*((8*(Kp + Kd*N))/(Ts*Ts) - 2*Ki*N))/(2*(N + 2/Ts));
        b2 = (Ts*((4*(Kp + Kd*N))/(Ts*Ts) - (2*Ki + 2*Kp*N)/Ts + Ki*N))/(2*(N + 2/Ts));

        //convert u0 (voltage -12 to 12) to PWM value (between -128 and 127)
        if (u0 >= 0)
		{
			u0_PWM = (127.0/12.0)*u0;   //cast to integer
			u0_PWM &= 0x7F;         //Positive number MSB = 0
		}
		else
		{
			u0 = -u0;
			u0_PWM = (127.0/12.0)*u0;   //cast to integer
			u0_PWM |= 0x80;         //Negative number MSB = 1
		}

        // Limit lower PWM
	   if (u0_PWM & 0x80) {//Negative PWM
		(u0_PWM & 0x7F) < PWM_LIM_PAN_CW ? u0_PWM = 0 : u0_PWM;
	   } else {//Positive PWM
		u0_PWM < PWM_LIM_PAN_CCW ? u0_PWM = 0 : u0_PWM;
	   }

        pwm_pan = u0_PWM;
    }
}
/****************************** End Of Module *******************************/


void pid_controller_tilt(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : .
******************************************************************************/
{
    FP64 Kp    =      3.797;        //Proportional gain
    FP64 Ti    =      45.9571;		//Integral constant
    FP64 Td    =      0.071717;     //Derivative constant
    FP64 Ts    =      0.01;         //Sampling time

    FP64 N     =      20.4131;       //Cutoff frequency in rad/s of lowpass filter

    FP64 e = 0, w0 = 0, w1 = 0, w2 = 0, u0 = 0;

    INT8U u0_PWM;

    FP64 r = 0;         //reference

    FP64 Ki = Kp/Ti;        //Integral gain    --set to zero when out of bounds,
    FP64 Kd = Kp*Td;        //Derivative gain

    INT8S y_third_degree;
    FP32  y_rad = 0;

    FP64 a0 = 1;
    FP64 a1 = -4/(Ts*(N+2/Ts));
    FP64 a2 = -(N-2/Ts)/(N+2/Ts);
    FP64 b0 = (Ts*((2*Ki + 2*Kp*N)/Ts + (4*(Kp + Kd*N))/(Ts*Ts) + Ki*N))/(2*(N + 2/Ts));
    FP64 b1 = -(Ts*((8*(Kp + Kd*N))/(Ts*Ts) - 2*Ki*N))/(2*(N + 2/Ts));
    FP64 b2 = (Ts*((4*(Kp + Kd*N))/(Ts*Ts) - (2*Ki + 2*Kp*N)/Ts + Ki*N))/(2*(N + 2/Ts));

    INT8U y_rad_array_TEMP[] = "None   ";
    INT8U *ptr_y_rad_array_TEMP = &y_rad_array_TEMP;

    INT16S y_rad_temp = 0;
    INT8U uart_counter = 0;
    const INT8U ch = 'T';

    while (1)
    {
    	r += joystick_tilt;

        //Blocks task if queue is empty, otherwise save next value from queue in receivedValue
        xQueueReceive( placementTiltFrameQueue, &y_third_degree,  portMAX_DELAY );

        // Temp value for logging position
        y_rad_temp += y_third_degree;

        //Convert from encoder ticks to rad. Take the CW and CCW resolution into account.
        if (y_third_degree > 0) {
        	y_rad += (2*PI/TICKS_TILT_CCW)*y_third_degree;	//Convert from positive ticks to rad.
        } else {
        	y_rad += (2*PI/TICKS_TILT_CW)*y_third_degree;   //Convert from negative ticks to rad.
        }

        // Write value to uart for logging
        uart_counter++;
        if (uart_counter >= 50) {
			uart_counter = 0;
			floatToArray(y_rad, ptr_y_rad_array_TEMP);
			log_data(ch, ptr_y_rad_array_TEMP);
        }


        //update variables,
        w2 = w1;
        w1 = w0;

        e = r - y_rad;          //compute new error

        w0 = e - a1*w1 -a2 * w2;

        u0 = b0 * w0 + b1 * w1 + b2 * w2;      //difference equation,


        //Anti-windup
        if ((u0 > UMAX) || (u0 < UMIN) )
        {
            Ki = 0;
            b0 = (Ts*((2*Ki + 2*Kp*N)/Ts + (4*(Kp + Kd*N))/(Ts*Ts) + Ki*N))/(2*(N + 2/Ts));
            b1 = -(Ts*((8*(Kp + Kd*N))/(Ts*Ts) - 2*Ki*N))/(2*(N + 2/Ts));
            b2 = (Ts*((4*(Kp + Kd*N))/(Ts*Ts) - (2*Ki + 2*Kp*N)/Ts + Ki*N))/(2*(N + 2/Ts));
            u0 = b0*w0 + b1 * w1 + b2 * w2;      //difference equation,
        }


        //saturation
        if (u0 > UMAX)
        {
            u0 = UMAX;          //Limit to PWM range,
        }
        if (u0 < UMIN)
        {
            u0 = UMIN;          //Limit to PWM range,
        }


        //reset values
        Ki = Kp/Ti;        //Integral gain
        b0 = (Ts*((2*Ki + 2*Kp*N)/Ts + (4*(Kp + Kd*N))/(Ts*Ts) + Ki*N))/(2*(N + 2/Ts));
        b1 = -(Ts*((8*(Kp + Kd*N))/(Ts*Ts) - 2*Ki*N))/(2*(N + 2/Ts));
        b2 = (Ts*((4*(Kp + Kd*N))/(Ts*Ts) - (2*Ki + 2*Kp*N)/Ts + Ki*N))/(2*(N + 2/Ts));

        //convert u0 (voltage -12 to 12) to PWM value (between -128 and 127)
        if (u0 >= 0)
        {
        	u0_PWM = (127.0/12.0)*u0;   //cast to integer
            u0_PWM &= 0x7F;         //Positive number MSB = 0
        }
        else
        {
        	u0 = -u0;
        	u0_PWM = (127.0/12.0)*u0;   //cast to integer
            u0_PWM |= 0x80;         //Negative number MSB = 1
        }

        // Limit lower PWM
        if (u0_PWM & 0x80) {//Negative PWM
        	(u0_PWM & 0x7F) < PWM_LIM_TILT_CW ? u0_PWM = 0 : u0_PWM;
        } else {//Positive PWM
        	u0_PWM < PWM_LIM_TILT_CCW ? u0_PWM = 0 : u0_PWM;
        }

        pwm_tilt = u0_PWM;
    }
}

void log_data(INT8U ch, INT8U *ptr_data) {
	INT8U col = ':';

	xQueueSendToBack(uart0Queue, &ch, 0);
	xQueueSendToBack(uart0Queue, &col, 0);

	for (INT8U i = 0; i < 7; i++) {
		col = ptr_data[i];
		xQueueSendToBack(uart0Queue, &col, 0);
	}
	col = '\n';
	xQueueSendToBack(uart0Queue, &col, 0);
	return;
}

/****************************** End Of Module *******************************/
