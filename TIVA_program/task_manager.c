/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: gpio.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 200403  FBS   Module created.
* 200505  FBS   Task_manager edited - spi_task and uart0_task included
*
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
INT8U pwm_pan = 'a';    //0;
INT8U pwm_tilt = 'b';    //0;

/*****************************   Functions   *******************************/


void driver_test_led()
/*****************************************************************************
*   Function : Function used to test tasks
*****************************************************************************/
{
    static const TickType_t xDelay500ms = pdMS_TO_TICKS( 500 );
    BaseType_t xStatus;
    //INT8U test_data = 'V';

    while(1)
    {

        GPIO_PORTF_DATA_R ^= (0x02); //Clears LED on tiva//red/turns on RED LED on green board

        //xStatus = xQueueSendToBack( uart0Queue, &test_data, 0);

        vTaskDelay( xDelay500ms );

    }
}

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
                 4,
                 NULL);

//    xTaskCreate( uart0_task,
//                 "Uart0",
//                 128,//configMINIMAL_STACK_SIZE,
//                 NULL,
//                 2,
//                 NULL);

//    xTaskCreate( driver_test_led,
//                 "Test",
//                 configMINIMAL_STACK_SIZE,
//                 NULL,
//                 1,
//                 NULL);

    xTaskCreate( pid_controller_tilt,
                 "tilt",
                 128,
                 NULL,
                 3,
                 NULL);

    xTaskCreate( pid_controller_pan,
                 "Pan",
                 128,
                 NULL,
                 3,
                 NULL);

//    xTaskCreate( uart0_rx_task,
//                     "Rx",
//                     configMINIMAL_STACK_SIZE,
//                     NULL,
//                     1,
//                     NULL);

    xTaskCreate(joystick_task,
				"joystick",
				configMINIMAL_STACK_SIZE,
				NULL,
				2,
				NULL);

    vTaskStartScheduler();


    for( ;; );
}





