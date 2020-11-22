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

PID_LOCATION pid_loc;
PID_INCREMENT pid_inc;

/*
*********************************************************************************************************
* 功  能：位置式PID初始化
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/11/22
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
		
	pid_loc.kp = 25;
	pid_loc.ki = 1.75;
	pid_loc.kd = 250;
}

/*
*********************************************************************************************************
* 功  能：位置式PID算法
* 形  参：set    - 设定值
*		  actual - 实际值
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/11/22
*********************************************************************************************************
*/

void Pid_Location(float set, float actual)
{ 
	float out = 0;
	 
    if(time_pid < 5000)
    {
        return ;       
    }
    
    time_pid = 0;
    
    pid_loc.set = set;
	pid_loc.actual = actual;
	pid_loc.err = pid_loc.set - pid_loc.actual;
	pid_loc.integral += pid_loc.err;

	if(pid_loc.integral > 100.0)
		pid_loc.integral = 100.0;
	if(pid_loc.integral < -100.0)
		pid_loc.integral = -100.0;
    
    if(pid_loc.set > 100.0)
		pid_loc.ki = 1.75*(pid_loc.set/100.0);
	else
		pid_loc.ki = 1.75;
    	
	out = pid_loc.kp*pid_loc.err + pid_loc.ki*pid_loc.integral + pid_loc.kd*(pid_loc.err-pid_loc.err1) + 5;

	pid_loc.err1 = pid_loc.err;

	if(out <= 0.0)
		out = 0.0;
	if(out >= 1000.0)
		out = 1000.0;
	pid_loc.out = out;
}

/*
*********************************************************************************************************
* 功  能：增量式PID初始化
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/11/22
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
		
	pid_inc.kp = 50;
	pid_inc.ki = 10;
	pid_inc.kd = 10;
}

/*
*********************************************************************************************************
* 功  能：增量式PID算法
* 形  参：set    - 设定值
*		  actual - 实际值
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/11/22
*********************************************************************************************************
*/

void Pid_Increment(float set, float actual)
{   
	float out = 0;
	
    if(time_pid < 5000)
    {
        return ;       
    }
    
    time_pid = 0;
    
    pid_inc.set = set;
    pid_inc.actual = actual;
    pid_inc.err = pid_inc.set - pid_inc.actual;
    
    out += pid_inc.kp*(pid_inc.err-pid_inc.err1) + pid_inc.ki*pid_inc.err + pid_inc.kd*(pid_inc.err-2*pid_inc.err1+pid_inc.err2);
    
    pid_inc.err2 = pid_inc.err1;
    pid_inc.err1 = pid_inc.err;
    
    if(out <= 0.0)
		out = 0.0;
	if(out >= 1000.0)
		out = 1000.0;
    pid_inc.out = out;
}

