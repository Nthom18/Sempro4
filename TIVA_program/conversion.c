/*****************************************************************************
* University of Southern Denmark
* RB-PRO4 - Group 4
* Semesterproject in control and regulation of robotic systems
* Module: gpio.c
* Created 17/05/2020
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "conversion.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void textToArray(INT8U array[], INT8U length, INT8U *ptr_Array)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    for (int i = 0; i < length; i++)
    {
        ptr_Array[i] = array[i];
    }
}




void numberToArray(INT16S number, INT8U *ptr_tempArray)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	if (number < 0) {
		ptr_tempArray[0] = '-';
		number = -number;
	}
	else
	{
		ptr_tempArray[0] = ' ';
	}
    if (number >= 10000)
    {
        ptr_tempArray[1] = (number / 10000) + '0';
    }
    else
    {
        ptr_tempArray[1] = ' ';
    }
    if (number >= 1000)
    {
        ptr_tempArray[2] = ((number % 10000) / 1000) + '0';
    }
    else
    {
        ptr_tempArray[2] = ' ';
    }
    if (number >= 100)
    {
        ptr_tempArray[3] = ((number % 1000) / 100) + '0';      //hundreds
    }
    else
    {
        ptr_tempArray[3] = ' ';
    }
    if (number >= 10)
    {
        ptr_tempArray[4] = ((number % 100) / 10) + '0';
    }
    else
    {
        ptr_tempArray[4] = ' ';
    }
    ptr_tempArray[5] = (number % 10) + '0';
}


void floatToArray(FP64 number, INT8U *ptr_tempArray)  //numbers from 0.01 to 99.99
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    FP64 tempNumber = number;
    INT8U counterTens = 0;
    INT8U counterOnes = 0;
    INT8U counterTenths = 0;
    INT8U counterHundredths = 0;

    if (number < 0) {
        	ptr_tempArray[0] = '-';
        	tempNumber = -tempNumber;
        } else {
        	ptr_tempArray[0] = ' ';
        }

    while (tempNumber >= 10)
    {
        counterTens++;
        tempNumber = tempNumber - 10.0;
    }
    while (tempNumber >= 1)
    {
        counterOnes++;
        tempNumber = tempNumber - 1.0;
    }
    while (tempNumber >= 0.1)
    {
        counterTenths++;
        tempNumber = tempNumber - 0.1;
    }
    while (tempNumber >= 0.005)
    {
        if (counterHundredths != 9)
        {
        counterHundredths++;
        }

        tempNumber = tempNumber - 0.01;
    }

    if (counterTens > 0)
    {
        ptr_tempArray[1] = counterTens + '0';     //tens
    }
    else
    {
        ptr_tempArray[1] = ' ';
    }
    if (counterOnes >= 0)
    {
        ptr_tempArray[2] = counterOnes + '0';  //ones
    }
    else
    {
        ptr_tempArray[2] = '0';
    }

    ptr_tempArray[3] = '.';

    if (counterTenths > 0)
    {
        ptr_tempArray[4] = counterTenths + '0';      //0.1
    }
    else
    {
        ptr_tempArray[4] = '0';
    }
    if (counterHundredths  > 0)
    {
        ptr_tempArray[5] = counterHundredths  + '0';      //0.01
    }
    else
    {
        ptr_tempArray[5] = '0';
    }
}

void floatToArrayTenThousand(FP32 number, INT8U *ptr_tempArray)  //numbers from 0.01 to 99999.99
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    FP32 tempNumber = number;
    INT8U counterTenThousands = 0;
    INT8U counterThousands = 0;
    INT8U counterHundreds = 0;
    INT8U counterTens = 0;
    INT8U counterOnes = 0;
    INT8U counterTenths = 0;
    INT8U counterHundredths = 0;

    if (number < 0) {
    	ptr_tempArray[0] = '-';
    	number = -number;
    } else {
    	ptr_tempArray[0] = ' ';
    }

    while (tempNumber >= 10000)
    {
        counterTenThousands++;
        tempNumber = tempNumber - 10000.0;
    }
    while (tempNumber >= 1000)
    {
        counterThousands++;
        tempNumber = tempNumber - 1000.0;
    }
    while (tempNumber >= 100)
    {
        counterHundreds++;
        tempNumber = tempNumber - 100.0;
    }
    while (tempNumber >= 10)
    {
        counterTens++;
        tempNumber = tempNumber - 10.0;
    }
    while (tempNumber >= 1)
    {
        counterOnes++;
        tempNumber = tempNumber - 1.0;
    }
    while (tempNumber >= 0.1)
    {
        counterTenths++;
        tempNumber = tempNumber - 0.1;
    }
    while (tempNumber >= 0.01)
    {
        if (counterHundredths != 9)
        {
        counterHundredths++;
        }
        tempNumber = tempNumber - 0.005;
    }

    if (counterTenThousands > 0)
    {
        ptr_tempArray[1] = counterTenThousands + '0';     //ten thousands
    }
    else
    {
        ptr_tempArray[1] = ' ';
    }
    if ((counterThousands > 0) | (counterTenThousands > 0))
    {
        ptr_tempArray[2] = counterThousands + '0';     //thousands
    }
    else
    {
        ptr_tempArray[2] = ' ';
    }
    if ((counterHundreds >0) | (counterThousands > 0) | (counterTenThousands > 0))
    {
        ptr_tempArray[3] = counterHundreds + '0';     //hundreds
    }
    else
    {
        ptr_tempArray[3] = ' ';
    }
    if ((counterTens > 0) | (counterHundreds > 0) | (counterThousands > 0) | (counterTenThousands > 0))
    {
        ptr_tempArray[4] = counterTens + '0';     //tens
    }
    else
    {
        ptr_tempArray[4] = ' ';
    }
    if (counterOnes >= 0)
    {
        ptr_tempArray[5] = counterOnes + '0';  //ones
    }
    else
    {
        ptr_tempArray[5] = '0';
    }

    ptr_tempArray[6] = '.';

    if (counterTenths > 0)
    {
        ptr_tempArray[7] = counterTenths + '0';      //0.1
    }
    else
    {
        ptr_tempArray[7] = '0';
    }
    if (counterHundredths  > 0)
    {
        ptr_tempArray[8] = counterHundredths  + '0';      //0.01
    }
    else
    {
        ptr_tempArray[8] = '0';
    }

}
