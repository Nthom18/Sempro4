#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include <stdint.h>
#include <stdio.h>
#include <task_manager.h>


#include "gpio.h"
#include "glob_def.h"
#include "spi.h"
#include "uart0.h"

/*****************************    Defines    *******************************/
#define DATA_SIZE   8
#define CLK_PSCL    1
#define CLK_PH      0
#define CLK_PO      0

#define BAUD_RATE   9600
#define DATA_BITS   8
#define STOP_BITS   1
#define PARITY      'n'

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

int main(void)
{
    //Initialize GPIO
    init_gpio();

    //Initialize SPI
    spi_init(DATA_SIZE, CLK_PSCL, CLK_PH, CLK_PO);

    //Initialize uart0
    uart0_init(BAUD_RATE, DATA_BITS, STOP_BITS, PARITY);

    //FreeRTOS task scheduler
    task_manager();

	return 0;
}
