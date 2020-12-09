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
	float set;                                                                   /* �趨ֵ             */
	float actual;	                                                             /* ʵ��ֵ             */
	float err;			                                                         /* ��ǰ���           */
	float err1;                                                                  /* �ϴ����           */
	float kp, ki, kd;	                                                         /* P, I, D ϵ��       */
	float out;	                                                                 /* ���ֵ             */
	float integral;                                                              /* ����ֵ             */
}PID_LOCATION;
#endif

#if 0
typedef struct
{
    float set;                                                                   /* �趨ֵ             */
    float actual;                                                                /* ʵ��ֵ             */
    float err;                                                                   /* ��ǰ���           */
    float err1;                                                                  /* �ϴ����           */
    float err2;                                                                  /* ���ϴ����         */
    float kp, ki, kd;                                                            /* P, I, D ϵ��       */
    float out;                                                                   /* ���ֵ             */
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

