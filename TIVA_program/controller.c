/*****************************************************************************
* University of Southern Denmark
*
* MODULENAME.: controller.c
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
* 200512  FBS   Module created.
*
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
#define INCREMENT (PI/4.0)

//Queues for samples from frame 1 and 2
extern QueueHandle_t placementPanFrameQueue, placementTiltFrameQueue;

//PWM Motor registers
extern INT8U pwm_pan;
extern INT8U pwm_tilt;


extern QueueHandle_t uart0Queue;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

extern INT8U ref_tilt;


/*****************************   Functions   *******************************/
void pid_controller_pan(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : .
******************************************************************************/
{
    FP64 Kp    =      4.3555;        //Proportional gain
    FP64 Ti    =      51.5717;
    FP64 Td    =      0.11358;       //Derivative constant
    FP64 Ts    =      0.01;          //Sampling time

    FP64 N     =      28.8267;       //Order of lowpass filter


    FP64 e = 0, w0 = 0, w1 = 0, w2 = 0, u0 = 0;

    INT8U u0_PWM;

    FP64 r = 0;         //reference

    //FP64 y;         //output

    //user adjustable parameters -- should be change,
    FP64 Ki = Kp/Ti;        //Integral gain    --set to zero when out of bounds,
    FP64 Kd = Kp*Td;        //Derivative gain


    INT8U reference;
    INT8S y_third_degree;
    FP32  y_rad = 0;
    BaseType_t xStatus;

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

    while (1)
    {
        //Blocks task if queue is empty, otherwise save next value from queue in receivedValue
        xStatus = xQueueReceive( placementPanFrameQueue, &y_third_degree,  portMAX_DELAY );

        // Temp value for logging position
        y_rad_temp += y_third_degree;

        //Convert from 1/3 degrees to rad
        y_rad += (2*PI/240)*y_third_degree;    //Possibly added, and MSB possibly sign bit OBS!

        // Write temp value to uart for logging
//      uart_counter++;
//		if (uart_counter >= 50) {
//			uart_counter = 0;
//			numberToArray(y_rad_temp, ptr_y_rad_array_TEMP);
//			uart0_putca(y_rad_array_TEMP);
//			uart0_putc('\n');
//		}

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
    FP64 Kp    =      10; //3.797;        //Proportional gain
    FP64 Ti    =      10; //45.9571;
    FP64 Td    =      0.071717;     //Derivative constant
    FP64 Ts    =      0.01;        //Sampling time

    FP64 N     =      20.4131;       //Cutoff frequency in rad/s of lowpass filter

    FP64 e = 0, w0 = 0, w1 = 0, w2 = 0, u0 = 0;

    INT8U u0_PWM;

    FP64 r = 0;         //reference

    //FP64 y;         //output

    //user adjustable parameters -- should be change,
    FP64 Ki = Kp/Ti;        //Integral gain    --set to zero when out of bounds,
    FP64 Kd = Kp*Td;        //Derivative gain


    INT8U reference;
    INT8S y_third_degree;
    FP32  y_rad = 0;
    BaseType_t xStatus;

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

    while (1)
    {
    	//r = ref_tilt * INCREMENT;

        //Blocks task if queue is empty, otherwise save next value from queue in receivedValue
        xStatus = xQueueReceive( placementTiltFrameQueue, &y_third_degree,  portMAX_DELAY );

        // Temp value for logging position
        y_rad_temp += y_third_degree;

        //Convert from 1/3 degrees to rad
        y_rad += (2*PI/270)*y_third_degree;    //Possibly added, and MSB possibly sign bit OBS!

        // Write temp value to uart for logging
        uart_counter++;
        if (uart_counter >= 50) {
			uart_counter = 0;
			numberToArray(y_rad_temp, ptr_y_rad_array_TEMP);
			uart0_putca(y_rad_array_TEMP);
			uart0_putc('\n');
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
        pwm_tilt = u0_PWM;
    }
}
/****************************** End Of Module *******************************/
