/*
*********************************************************************************************************
* HeadFile
*********************************************************************************************************
*/

#include "pid.h"

/*
*********************************************************************************************************
* Struct
*********************************************************************************************************
*/

#if 1
PID_LOCATION pid_loc;
#endif

#if 0
PID_INCREMENT pid_inc;
#endif

/*
*********************************************************************************************************
* Extern
*********************************************************************************************************
*/

extern int16_t para_p;
extern int16_t para_i;
extern int16_t para_d;
extern int16_t para_auto;
extern int16_t para_oud;
extern void Data_Save(void);

#if 1

/*
*********************************************************************************************************
* 功  能：位置型PID初始化
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/12/2
*********************************************************************************************************
*/

void Pid_Loc_Init(void)
{
	pid_loc.set = 0;
	pid_loc.actual = 0;
	pid_loc.err = 0;
	pid_loc.err1 = 0;
	pid_loc.integral = 0;
	pid_loc.out = 0;
		
	pid_loc.kp = para_p;
	pid_loc.ki = para_i;
	pid_loc.kd = para_d;
}

/*
*********************************************************************************************************
* 功  能：位置型PID算法
* 形  参：set    - 设定值
*		  actual - 实际值
* 返回值：功率
* 备  注：None
* 作  者：LG
* 日  期：2020/12/2
*********************************************************************************************************
*/

uint16_t Pid_Location(float set, float actual)
{    
    if(time_pid < 5000)
    {
        return (uint16_t)pid_loc.out;       
    }
    
    pid_loc.set = set;
	pid_loc.actual = actual;
    pid_loc.err = pid_loc.set - pid_loc.actual;
    
    if(para_oud)
        pid_loc.err = -pid_loc.err;
    
    if(para_auto)
    {
        if(Pid_Auto(&pid_loc.kp, &pid_loc.ki, &pid_loc.kd))
        {
            para_auto = 0;
            para_p = pid_loc.kp;
            para_i = pid_loc.ki;
            para_d = pid_loc.kd;
            Data_Save();
        }
        time_pid = 0;
        return (uint16_t)pid_loc.out;
    }
    
    if(fabs(pid_loc.err) <= 10) 
        pid_loc.integral += pid_loc.err;
    else
        pid_loc.integral = 0;
    
	if(pid_loc.integral > 1000)
		pid_loc.integral = 1000;
	if(pid_loc.integral < -100)
		pid_loc.integral = -100;
    	
	pid_loc.out = pid_loc.kp*pid_loc.err + pid_loc.ki*pid_loc.integral + pid_loc.kd*(pid_loc.err-pid_loc.err1);
    
	pid_loc.err1 = pid_loc.err;

	if(pid_loc.out <= 0)
		pid_loc.out = 0;
	if(pid_loc.out >= 1000)
		pid_loc.out = 1000;
    
    time_pid = 0;
    
    return (uint16_t)pid_loc.out;
}

#endif

#if 0

/*
*********************************************************************************************************
* 功  能：增量型PID初始化
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/12/2
*********************************************************************************************************
*/

void Pid_Inc_Init(void)
{
	pid_inc.set = 0;
	pid_inc.actual = 0;
	pid_inc.err = 0;
	pid_inc.err1 = 0;
	pid_inc.err2 = 0;
	pid_inc.out = 0;
		
	pid_inc.kp = para_p;
	pid_inc.ki = para_i;
	pid_inc.kd = para_d;
}

/*
*********************************************************************************************************
* 功  能：增量型PID算法
* 形  参：set    - 设定值
*		  actual - 实际值
* 返回值：功率
* 备  注：None
* 作  者：LG
* 日  期：2020/12/2
*********************************************************************************************************
*/

uint16_t Pid_Increment(float set, float actual)
{  
    if(time_pid < 5000)
    {
        return (uint16_t)pid_inc.out;     
    }
    
    pid_inc.set = set;
    pid_inc.actual = actual;
    pid_inc.err = pid_inc.set - pid_inc.actual;
    
    pid_inc.out += pid_inc.kp*(pid_inc.err-pid_inc.err1) + pid_inc.ki*pid_inc.err + pid_inc.kd*(pid_inc.err-2*pid_inc.err1+pid_inc.err2);
    
    pid_inc.err2 = pid_inc.err1;
    pid_inc.err1 = pid_inc.err;
    
    if(pid_inc.out <= 0)
		pid_inc.out = 0;
	if(pid_inc.out >= 1000)
		pid_inc.out = 1000;
    
    time_pid = 0;
    
    return pid_inc.out;
}

#endif

#if 1

/*
*********************************************************************************************************
* 功  能：PID自整定算法
* 形  参：None
* 返回值：None
* 备  注：0: 未完成
*         1: 完成
* 作  者：LG
* 日  期：2020/12/7
*********************************************************************************************************
*/

uint8_t Pid_Auto(float* kp_value, float* ki_value, float* kd_value)
{
    static uint16_t counter = 0;
    static uint8_t zero_flag = 0;
    static uint16_t time_low = 0;
    static uint16_t time_high = 0;
    static int16_t temp_low = -100;
    static int16_t temp_high = -100;
    float ku = 0, tu = 0;
    
    counter+=5;
 
    if(pid_loc.err > 0)
        pid_loc.out = 1000;
    else
        pid_loc.out = 0;
    
    switch(zero_flag)
    {
        case 0: if(pid_loc.actual < pid_loc.set)
                    zero_flag = 1;
                break;
        case 1: if(pid_loc.actual >= pid_loc.set)
                    zero_flag = 2;
                break;
        case 2: if(pid_loc.actual < pid_loc.set)
                    zero_flag = 3;
                break;
        case 3: if(pid_loc.actual >= pid_loc.set)
                {
                    zero_flag = 4;
                    time_low = counter;
                }
                if(temp_low == -100)
                    temp_low = pid_loc.actual;
                if(temp_low > pid_loc.actual)
                    temp_low = pid_loc.actual;                
                break;
        case 4: if(pid_loc.actual < pid_loc.set)
                {
                    zero_flag = 5;
                    time_high = counter; 
                }
                if(temp_high == -100)
                    temp_high = pid_loc.actual;
                if(temp_high  < pid_loc.actual)
                        temp_high = pid_loc.actual;
                break;
        case 5: if(pid_loc.actual >= pid_loc.set)
                {
                    zero_flag = 6;
                    ku = 4*100/3.1415926/(temp_high - temp_low);
                    tu = 2 * (time_high - time_low);	
                    *kp_value = 0.6*ku*10;		
                    *ki_value = (0.6*ku*10) / (0.5*tu) * 20;
                    *kd_value = (0.6*ku*10) * (0.125*tu) / 20;
                    
                    if(*kp_value >= 999)
                        *kp_value = 999;
                    if(*kp_value <= 1)
                        *kp_value = 1;
                    
                    if(*ki_value >= 999)
                        *ki_value = 999;
                    if(*ki_value <= 1)
                        *ki_value = 1;
                    
                    if(*kd_value >= 999)
                        *kd_value = 999;
                    if(*kd_value <= 1)
                        *kd_value = 1;
                    
                    counter = 0;
                    zero_flag = 0;
                    time_low = 0;
                    time_high = 0;
                    temp_low = -100;
                    temp_high = -100;
                    
                    return 1;
                } 
                break;
    }
    
    return 0;
}

#endif

