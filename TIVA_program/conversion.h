/*****************************************************************************
* University of Southern Denmark
* RB-PRO4 - Group 4
* Semesterproject in control and regulation of robotic systems
* Module: conversion.h
* Created 17/05/2020
*****************************************************************************/
#ifndef CONVERSION_H_
#define CONVERSION_H_

/***************************** Include files *******************************/


/*****************************    Defines    *******************************/


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void textToArray(INT8U array[], INT8U length, INT8U *ptr_Array);
/*****************************************************************************
*   Input    : Array to moved to other array, length of the copied array, array to place array in
*   Output   : -
*   Function : Copy array to other array,
******************************************************************************/

void numberToArray(INT16S number, INT8U *ptr_tempArray);
/*****************************************************************************
*   Input    : Number to be converted, array to save converted number
*   Output   : -
*   Function : Conversion of integer (INT16U) to array of INT8U
******************************************************************************/

void floatToArray(FP64 number, INT8U *ptr_tempArray);
/*****************************************************************************
*   Input    : Number to be converted, array to save converted number
*   Output   : -
*   Function : Conversion of floating point (FP32) numbers from 0.01 to 99.99 to array of INT8U
******************************************************************************/

void floatToArrayTenThousand(FP32 number, INT8U *ptr_tempArray);  //numbers from 0.01 to 99999.99
/*****************************************************************************
*   Input    : Number to be converted, array to save converted number
*   Output   : -
*   Function : Conversion of floating point (FP32) numbers from 0.01 to 99999.99 to array of INT8U
******************************************************************************/
#endif /* CONVERSION_H_ */
