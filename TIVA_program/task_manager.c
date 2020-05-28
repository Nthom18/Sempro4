/*****************************************************************************
* University of Southern Denmark
* RB-PRO4 - Group 4
* Semesterproject in control and regulation of robotic systems
* Module: task_manager.c
* Created 03/04/2020
*****************************************************************************/

/* Standard includes. */
#include <stdio.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "task_manager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart0.h"
#include "spi.h"
#include "glob_def.h"
#include "controller.h"
#include "joystick.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

QueueHandle_t uart0Queue;

//Queues for samples from frame 1 and 2
QueueHandle_t placementPanFrameQueue;
QueueHandle_t placementTiltFrameQueue;


//PWM Motor 1 register
INT8U pwm_pan = 'a';
INT8U pwm_tilt = 'b';

/*****************************   Functions   *******************************/

void task_manager()
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{

    uart0Queue = xQueueCreate(48, sizeof(INT8U));
    placementPanFrameQueue = xQueueCreate(16, sizeof(INT8U));
    placementTiltFrameQueue = xQueueCreate(16, sizeof(INT8U));


    xTaskCreate( spi_task,
                 "SPI",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 3,
                 NULL);

    xTaskCreate( uart0_task,
                 "Uart0",
                 128,
                 NULL,
                 1,
                 NULL);

    xTaskCreate( pid_controller_tilt,
                 "tilt",
                 128,
                 NULL,
                 2,
                 NULL);

    xTaskCreate( pid_controller_pan,
                 "Pan",
                 128,
                 NULL,
                 2,
                 NULL);

    xTaskCreate(joystick_task,
				"joystick",
				configMINIMAL_STACK_SIZE,
				NULL,
				1,
				NULL);

    vTaskStartScheduler();


    for( ;; );
}





