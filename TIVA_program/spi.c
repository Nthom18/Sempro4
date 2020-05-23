/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: spi.c
 *
 * PROJECT....: EMP
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 *****************************************************************************
 * Date    Id          Change
 * YYMMDD
 * --------------------
 * 200306  CH          Module created.
 * 200505  CSN & FBS   SPI task created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include "emp_type.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#include "task_manager.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

//Queues for samples from frame 1 and 2
extern QueueHandle_t placementFrame1Queue;
extern QueueHandle_t placementFrame2Queue;


//Test
extern QueueHandle_t uart0Queue;
//End test

//PWM Motor registers
extern INT8U pwm_pan;
extern INT8U pwm_tilt;
/*****************************   Functions   *******************************/



extern void spi_init(INT8U DDS, INT8U CPSDVSR, BOOLEAN SPH, BOOLEAN SPO)
/*****************************************************************************
 *   Function : See module specification (.h-file).
 *****************************************************************************/
{
    //Note: further division can be done in the SSI2_CR0_R register if necessary

    //Enable and provide a clock to SSI module 2 in Run mode
    SYSCTL_RCGCSSI_R |= (1 << 2);

    //Enable and provide a clock to GPIO Port B in Run mode
    SYSCTL_RCGCGPIO_R |= (1 << 1);

    //Enable alternate functions for relevant pins
    GPIO_PORTB_AFSEL_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);

    //Enable SSI2Clk for PB4, SSI2Fss for PB5, SSI2Rx for PB6, and SSI2Tx for PB7
    GPIO_PORTB_PCTL_R = 0x22220000;

    //Enable digital function of PB4, PB5, PB6, and PB7
    GPIO_PORTB_DEN_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);

    //Enable pull-up resistors on PB4, PB5, PB6, and PB7
    GPIO_PORTB_PUR_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);

    //Enable master mode
    SSI2_CR1_R = 0x00000000;

    //CPU clock source
    SSI2_CC_R &= 0x0;

    // Load divisor into prescaler register
    SSI2_CPSR_R = CPSDVSR;  //SSInClk = SysClk / (CPSDVSR * (1 + SCR))

    //Set number of data bits
    SSI2_CR0_R |= DDS - 1;

    //Set polarity
    SSI2_CR0_R |= (SPH << 7);
    SSI2_CR0_R |= (SPO << 6);

    //NB!
    //SSI2_CR0_R |= 0x400;

    //Enable SSI
    SSI2_CR1_R |= 0x2;
}

extern void send_byte(INT8U data) {
    //Load FIFO transmit buffer with data
    SSI2_DR_R = (0x00FF & data);

    //wait for transmit to complete
    while ((SSI2_SR_R & 1) == 0)
        ;
}

extern INT8U receive_byte() {
    //Correct?
    INT8U data;

    //Load FIFO transmit buffer with data
    SSI2_DR_R = 0x00;

    //wait for receive to complete
    while ((SSI2_SR_R & 1) == 0);

    //Read bit 0 to 7
    data = (SSI2_DR_R & 0x00FF);
    return data;
}

extern INT8U sr_byte(INT8U data_send) {
    INT16U data_receive = 0;

    //Load FIFO transmit buffer with data
    SSI2_DR_R = data_send;

    //wait for send/receive to complete
    while ((SSI2_SR_R & 1) == 0)
        ;

    while((SSI2_SR_R & 0x04) == 0);     //wait until data is in the receive FIFO before reading

    //Read bit 0 to 7
    while (SSI2_SR_R & 0x04) {
        data_receive = (SSI2_DR_R & 0x00FF);
    }
    //data_receive = (SSI2_DR_R & 0x00FF);
    return data_receive;
}

extern INT16U sr_word(INT16U data_send16) {
    INT16U data_receive16 = 0;

    //Load FIFO transmit buffer with data
    SSI2_DR_R = data_send16;

    //wait for send/receive to complete
    while ((SSI2_SR_R & 1) == 0)
        ;

    while((SSI2_SR_R & 0x04) == 0);     //wait until data is in the receive FIFO before reading

    //Read word
    while (SSI2_SR_R & 0x04) {
        data_receive16 = SSI2_DR_R;
    }

    return data_receive16;
}

extern INT16U FPGA_update(INT16U PWM1, INT16U PWM2) {
    //Create 16 bit value with both PWM values.
    INT16U send_data = (PWM1 << 8) | PWM2;

    //Load FIFO transmit buffer with data
    SSI2_DR_R = send_data;

    //wait for send/receive to complete
    while ((SSI2_SR_R & 1) == 0);

    //wait until data is in the receive FIFO before reading
    while((SSI2_SR_R & 0x04) == 0);

    //Read word
    INT16U data_receive;
    while (SSI2_SR_R & 0x04) {
        data_receive = SSI2_DR_R;
    }

    return data_receive;

}


void spi_task()
/*****************************************************************************
 *   Function : See module specification (.h-file).
 *****************************************************************************/
{
    TickType_t xLastWakeTime;
    BaseType_t xStatus;

    INT16U received_data;
    INT8U received_data_frame1;
    INT8U received_data_frame2;

    static const TickType_t xDelay100ms = pdMS_TO_TICKS( 100 );

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {


        //Following function to transmit PWM signals and receive placement of M1 and M2
        received_data = FPGA_update(pwm_pan, pwm_tilt);



        //test
        //received_data = '9U';

        //if(received_data != 0){
            //received_data_frame1 = ((received_data & 0xFF00) >> 8)+ '0';
//            received_data_frame2 = (received_data & 0xFF) + '0';
//            xStatus = xQueueSendToBack( uart0Queue, &received_data_frame2, 0);
        //}
        //end test



        received_data_frame1 = ((received_data & 0xFF00) >> 8);
        received_data_frame2 = (received_data & 0xFF);

        //Store samples from frame 1 and 2 in respective queues,
        xStatus = xQueueSendToBack( placementFrame1Queue, &received_data_frame1, 0);
        xStatus = xQueueSendToBack( placementFrame2Queue, &received_data_frame2, 0);


        //Test code / send data via uart,
        xStatus = xQueueSendToBack( uart0Queue, &received_data_frame1, 0);
        xStatus = xQueueSendToBack( uart0Queue, &received_data_frame2, 0);
        //end test


        //test
        //received_data = 0;
        //end test


        vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(100) );
    }


}
void send_str(char *buffer) { //function for sending each byte of string one by one
    while (*buffer != 0) {
        send_byte(*buffer);
        buffer++;
    }
}

void spi_handler() {
    //Catch
    while (1)
        ;
}



/****************************** End Of Module *******************************/

