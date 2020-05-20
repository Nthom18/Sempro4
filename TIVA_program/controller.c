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
/*****************************    Defines    *******************************/
#define K_P         1000    //Proportional gain
#define T_D         2       //Derivative constant
#define T           0.01    //Sampling time
#define N           2       //Order of lowpass filter

#define UMAX        127
#define UMIN        -128

//Queues for samples from frame 1 and 2
extern QueueHandle_t placementFrame1Queue, placementFrame2Queue;
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
FP64 e2, e1, e0, u2, u1, u0;

FP64 r;         //referance

FP64 y;         //output

//user adjustable parameters -- should be change,
FP64 Kp = 10;       //proportional gain,
FP64 Ki = 1;        //Integral gain    --set to zero when out of bounds,
FP64 Kd = 1;        //Derivative gain



FP64 Ts = 0.01;         //Sampling time,


/*****************************   Functions   *******************************/

void pid_controller_M1(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : .
******************************************************************************/
{
    INT8U receivedValue;
    BaseType_t xStatus;

    FP64 a0 = (1+N*Ts);
    FP64 a1 = -(2 + N*Ts);
    FP64 a2 = 1;
    FP64 b0 = Kp*(1+N*Ts)+Ki*Ts*(1+N*Ts)+Kd*N;
    FP64 b1 = Kp + Kd*N;
    FP64 b2 = Kp + Kd*N;

    FP64 ku1 = a1/a0, ku2 = a2/a0, ke0 = b0/a0, ke1 = b1/a0, ke2 = b2/a0;

    while (1)
    {
        //Blocks task if queue is empty, otherwise save next value from queue in receivedValue
        xStatus = xQueueReceive( placementFrame1Queue, &receivedValue,  portMAX_DELAY );
        //update variables,
        e2 = e1;
        e1 = e0;
        u2 = u1;
        u1 = u0;

        y = receivedValue;          //y equal plan output

        e0 = r - y;                 //compute new error


        //Anti-windup
        if ((u0 > UMAX) | (u0 < UMIN) )
        {
            Ki = 0;
            b0 = Kp*(1+N*Ts)+Ki*Ts*(1+N*Ts)+Kd*N;
            ke0 = b0/a0;
        }

        u0 = -ku1*u1 - ku2*u2 + ke0*e0 + ke1*e1 + ke2*e2;       //difference equation,

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
        Ki = 1;  //Real value
        b0 = Kp*(1+N*Ts)+Ki*Ts*(1+N*Ts)+Kd*N;
        ke0 = b0/a0;
    }
}
/****************************** End Of Module *******************************/
