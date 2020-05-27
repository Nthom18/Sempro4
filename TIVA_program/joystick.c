#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
//#include "lcd.h"
#include "joystick.h"

FP32 joystick_pan = 0;
FP32 joystick_tilt = 0;

int done_cal = 0; //used to signal done calibrating

void init_adc()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Initializes adc-converter: ADC1
*              With sampling sequencer: SS2
*              sampling on AIN1 and AIN2 (PE1,PE2).
*              Sampling triggered by signal to PSSI (Processor Sample Sequence Initiate)
******************************************************************************/
{
    SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC1;    //Enable the ADC clock
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;   //Enable Port E

    GPIO_PORTE_DIR_R &= ~(0x0C);            //Set Port E direction

    GPIO_PORTE_AFSEL_R |= 0b00000110;       //Sets PE1 and PE2 to alternative function

    GPIO_PORTE_DEN_R &= ~(0b00000110);      //DISABLE digital-enable on pins whos running analog input  PE1 and PE2

    GPIO_PORTE_AMSEL_R |= 0b00000110;         //enable analog mode for PE1 and PE2

    //Config sequencer

    ADC1_ACTSS_R &= ~(1<<2); //disable sequencer SS2: (4 pins sampling) page 800
//    ADC1_EMUX_R |= 0xF<<8;   //continuously sample for SS2: (hench the shift 8 left)
    ADC1_EMUX_R |= 0x0<<8;   //ADCPSSI triggered for SS2 use ADC1_ADCPSSI_R |= 0x1<<2; //to trigger

//    ADC1_SSMUX2_R |= 0x2<<12; //sequencer 4th sample input from AIN2
//    ADC1_SSMUX2_R |= 0x1<<8; //sequencer 3th sample input from AIN1
    ADC1_SSMUX2_R |= 0x2<<4; //sequencer 2nd sample input from AIN2
    ADC1_SSMUX2_R |= 0x1<<0; //sequencer 1st sample input from AIN1

    ADC1_SSCTL2_R |= 0x1<<5;//END set at seq 2 in last nibble

    ADC1_ACTSS_R |= 1<<2; //enable sequencer SS2: (4 pins sampling) page 800


    //Joystick sw enabled
//    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;   //Enable Port D
//    GPIO_PORTD_DIR_R &= ~(0x01);   //Set PD0 direction to read
//    GPIO_PORTD_DEN_R |= 0x01;      //Digital enable PD0
//    GPIO_PORTD_PUR_R |= 0x01;       //enable pullup for PD0
}


void adc_get_samples(int* x, int* y)
/*****************************************************************************
*   Input    : reference to x and y coordinates
*   Output   : -
*   Function : Triggers a single sampling event and uses the result to
*              update values of referred coordinates
******************************************************************************/
{
    ADC1_PSSI_R |= 0x1<<2;//Get another sample from sequencer 2

    while(ADC1_SSFSTAT2_R & 0x1<<8);//wait while the FIFO is empty (9 to 11 cycles)

    *x = ADC1_SSFIFO2_R;//Dequeue element

    while(ADC1_SSFSTAT2_R & 0x1<<8);//wait while the FIFO is empty (9 to 11 cycles)

    *y = ADC1_SSFIFO2_R;//Dequeue element
}

enum joystick_states{IDLE, UNCALIBRATED, CALIBRATING, ACTIVE};
volatile int joy_sw = 0;

int joystick_state = IDLE;
int tempx = 0;
int tempy = 0;

void joystick_task()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : joystick task
******************************************************************************/
{
    while(1)
    {
        switch( joystick_state )
        {
            case IDLE:
                if (!(GPIO_PORTF_DATA_R & 0x01))//skips calibration
                {
//              wait( 1000 );
                    GPIO_PORTF_DATA_R = 0x02;
                    joystick_state = UNCALIBRATED;
                }
                if(!(GPIO_PORTF_DATA_R & 0x10))
                {
                    GPIO_PORTF_DATA_R = 0x04;
                    joystick_state = CALIBRATING;

                }
            break;
            case UNCALIBRATED:
                adc_get_samples(&tempx, &tempy);
                //joystick_hard_offset(&tempx, &tempy);		//Works without calibration on Christopher's Tiva
                //convert;
                joystick_pan  = (tempx - 2009) / 32404.0;
                joystick_tilt = (tempy - 2123) / 34242.0;

                if ((tempx - 2009) < 100 && (tempx - 2009) > -100) {
                	joystick_pan = 0;
                }
                if ((tempy - 2123) < 100 && (tempy - 2123) > -100) {
					joystick_tilt = 0;
				}

                break;
            case CALIBRATING:
//                joystick_calibrate();
                if(done_cal == 1)
                {
                    joystick_state = ACTIVE;
                }
            break;
            case ACTIVE:
                adc_get_samples(&tempx, &tempy);
//                joystick_offset_coordinates(&tempx, &tempy);
                //sleep for a while?

            break;
        }
    }
}
int cal_rest_x = 0;
int cal_rest_y = 0;

int xhigh_divisor = 1;
int yhigh_divisor = 1;

void joystick_hard_offset(int* x, int* y)
{
    cal_rest_x = 2009; //223;
    cal_rest_y = 2123; //248;
    xhigh_divisor = (4094 - cal_rest_x) / cal_rest_x; //14;
    yhigh_divisor = (4095 - cal_rest_y) / cal_rest_y; //11;

    if(*x < cal_rest_x)
    {
        *x = *x;
    }
    else
    {
        tempx = (tempx - cal_rest_x) / xhigh_divisor + cal_rest_x;
    }
    if(*y < cal_rest_y)
    {
        *y = *y;
    }
    else
    {
        tempy = (tempy - cal_rest_y) / yhigh_divisor + cal_rest_y;
    }
}





























///******************************************************/
////Following not in use at the moment
///******************************************************/
//
//
//
//
//
//
//
//int xlow_divisor = 1;
//int ylow_divisor = 1;
//
//
//enum calibration_states{FIND_REST, FIND_XMIN, FIND_XMAX, FIND_YMIN, FIND_YMAX, CANCEL, COMMIT};
//int cal_timeout = 0;
//int cal_state = 0;
//
//int cal_xmin = 0;
//int cal_xmax = 0;
//int cal_ymin = 0;
//int cal_ymax = 0;
//int cal_dump = 0;   //used to dump excess coordinate
//
//#define RESOLUTION 200
//int pressed = 0;
//void joystick_calibrate(void)
//{
//    //TODO: ALL CAL_ VALUES SHOULD PROBABLY BE CONSIDERED TEMP VALUES AND NOT TO BE TOUCH BY-
//    //-OTHER FUNCTIONS!
////    joy_sw = btn;
//    if(cal_timeout == 1)
//    {
//        cal_state = CANCEL;
//    }
//    switch(cal_state)
//    {
//        case FIND_REST:
//            if((GPIO_PORTF_DATA_R & 0x10))//sw1 = 0
//            {
//                pressed = 0;
//            }
//            if(pressed == 0)
//            {
//
//                if(!(GPIO_PORTF_DATA_R & 0x10))//sw1 = 1
//                {
//                    pressed = 1;
//                    adc_get_samples(&cal_rest_x, &cal_rest_y);
//
//                    GPIO_PORTF_DATA_R = 0x02;
//                    cal_state = FIND_XMIN;
//                }
//            }
//        break;
//        case FIND_XMIN:
//            if((GPIO_PORTF_DATA_R & 0x10))//sw1 = 0
//            {
//                pressed = 0;
//            }
//            if(pressed == 0)
//            {
//
//                if(!(GPIO_PORTF_DATA_R & 0x10))//sw1 = 1
//                {
//                    pressed = 1;
//                    adc_get_samples(&cal_xmin, &cal_dump);
//
//                    GPIO_PORTF_DATA_R = 0x08;
//                    cal_state = FIND_XMAX;
//                }
//            }
//        break;
//        case FIND_XMAX:
//            if((GPIO_PORTF_DATA_R & 0x10))//sw1 = 0
//            {
//                pressed = 0;
//            }
//            if(pressed == 0)
//            {
//                if(!(GPIO_PORTF_DATA_R & 0x10))//sw1 = 1
//                {
//                    pressed = 1;
//                    adc_get_samples(&cal_xmax, &cal_dump);
//
//                    //reset timeout timer
//                    cal_state = FIND_YMIN;
//                }
//            }
//        break;
//        case FIND_YMIN:
//            if((GPIO_PORTF_DATA_R & 0x10))//sw1 = 0
//            {
//                pressed = 0;
//            }
//            if(pressed == 0)
//            {
//
//                if(!(GPIO_PORTF_DATA_R & 0x10))//sw1 = 1
//                {
//                    pressed = 1;
//                    adc_get_samples(&cal_dump, &cal_ymin);
//                    //reset timeout timer
//                    cal_state = FIND_YMAX;
//                }
//            }
//        break;
//        case FIND_YMAX:
//            if((GPIO_PORTF_DATA_R & 0x10))//sw1 = 0
//            {
//                pressed = 0;
//            }
//            if(pressed == 0)
//            {
//
//                if(!(GPIO_PORTF_DATA_R & 0x10))//sw1 = 1
//                {
//                    pressed = 1;
//                    adc_get_samples(&cal_dump, &cal_ymax);
//
//                    //reset timeout timer
//                    cal_state = COMMIT;
//                }
//            }
//        break;
//        case COMMIT:
//            xlow_divisor =  ( cal_rest_x - cal_xmin )  / RESOLUTION;
//            if(xlow_divisor == 0)
//                xlow_divisor = 1;//backup value
//
//            xhigh_divisor = ( cal_xmax   - cal_rest_x) / RESOLUTION;
//            if(xhigh_divisor == 0)
//                xhigh_divisor = 1;//backup value
//
//            ylow_divisor =  ( cal_rest_y - cal_ymin )  / RESOLUTION;
//            if(ylow_divisor == 0)
//                ylow_divisor = 1;//backup value
//
//            yhigh_divisor = ( cal_ymax   - cal_rest_y) / RESOLUTION;
//            if(yhigh_divisor == 0)
//                yhigh_divisor = 1;//backup value
//
//            //stop timer
//            //SIGNAL END OF CALIBRATION TO ADC_STATE
//            cal_state = FIND_REST;
//        break;
//        case CANCEL:
//            //DO NOTHING
//            //stop timer
//            //SIGNAL END OF CALIBRATION TO ADC_STATE
//            cal_state = FIND_REST;
//            done_cal = 1;
//        break;
//    }
//}
//
//
//
//void joystick_offset_coordinates(int* x, int* y)
//{
//    if(*x < cal_rest_x)
//    {
//        *x = *x / xlow_divisor;
//    }
//    else
//    {
//        tempx = *x / xhigh_divisor + cal_rest_x / xlow_divisor;
//    }
//    if(*y < cal_rest_y)
//    {
//        *y = *y / ylow_divisor;
//    }
//    else
//    {
//        tempy = *y / yhigh_divisor + cal_rest_y / ylow_divisor;
//    }
//}
//
