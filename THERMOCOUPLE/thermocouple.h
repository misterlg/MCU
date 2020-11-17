#ifndef _THERMOCOUPLE_H_
#define _THERMOCOUPLE_H_

/*
*********************************************************************************************************
* HeadFile
*********************************************************************************************************
*/

#include "XX.h"
#include <math.h>

/*
*********************************************************************************************************
* Define
*********************************************************************************************************
*/

#define  ERR  -100

/*
*********************************************************************************************************
* Function
*********************************************************************************************************
*/

float K_VtoT(float mv);
float K_TtoV(float temp);
float S_VtoT(float mv);
float S_TtoV(float temp);
float R_VtoT(float mv);
float R_TtoV(float temp);
float B_VtoT(float mv);
float B_TtoV(float temp);
float N_VtoT(float mv);
float N_TtoV(float temp);
float E_VtoT(float mv);
float E_TtoV(float temp);
float J_VtoT(float mv);
float J_TtoV(float temp);
float T_VtoT(float mv);
float T_TtoV(float temp);

#endif

