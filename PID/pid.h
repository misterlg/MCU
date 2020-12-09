#ifndef _PID_H_
#define _PID_H_

/*
*********************************************************************************************************
* HeadFile
*********************************************************************************************************
*/

#include "hc32f005.h"
#include "systick.h"
#include <math.h>

/*
*********************************************************************************************************
* Typedef
*********************************************************************************************************
*/

#if 1
typedef struct
{
	float set;                                                                   /* 设定值             */
	float actual;	                                                             /* 实际值             */
	float err;			                                                         /* 当前误差           */
	float err1;                                                                  /* 上次误差           */
	float kp, ki, kd;	                                                         /* P, I, D 系数       */
	float out;	                                                                 /* 输出值             */
	float integral;                                                              /* 积分值             */
}PID_LOCATION;
#endif

#if 0
typedef struct
{
    float set;                                                                   /* 设定值             */
    float actual;                                                                /* 实际值             */
    float err;                                                                   /* 当前误差           */
    float err1;                                                                  /* 上次误差           */
    float err2;                                                                  /* 上上次误差         */
    float kp, ki, kd;                                                            /* P, I, D 系数       */
    float out;                                                                   /* 输出值             */
}PID_INCREMENT;
#endif

/*
*********************************************************************************************************
* Extern
*********************************************************************************************************
*/

#if 1
extern PID_LOCATION pid_loc;
#endif

#if 0
extern PID_INCREMENT pid_inc;
#endif

/*
*********************************************************************************************************
* Function
*********************************************************************************************************
*/

#if 1
void Pid_Loc_Init(void);
uint16_t Pid_Location(float set, float actual);
#endif

#if 0
void Pid_Inc_Init(void);
uint16_t Pid_Increment(float set, float actual);
#endif

#if 1
uint8_t Pid_Auto(float* kp_value, float* ki_value, float* kd_value);
#endif

#endif

