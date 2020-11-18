/*
*********************************************************************************************************
* HeadFile
*********************************************************************************************************
*/

#include "key.h"

/*
*********************************************************************************************************
* Global Variable
*********************************************************************************************************
*/

uint8_t key_flag = 0;                                                            /* 按键松开标志       */

/*
*********************************************************************************************************
* 功  能：Key检测 
* 形  参：None
* 返回值：0： 无按键按下
*		  1： KEY1短按
*		  2： KEY2短按
*		  3： KEY3短按
*		  4： KEY4短按
*		  10：KEY1长按
*		  20：KEY2长按
*		  30：KEY3长按
*		  40：KEY4长按
*		  23：组合按键2和3长按
* 备  注：None
* 作  者：LG
* 日  期：2020/11/18
*********************************************************************************************************
*/

uint8_t Key_Detect(void)
{
	static uint8_t key_value = 0;                                         /* 按键按下值                */
	static uint16_t counter_press = 0;                                    /* 按键按下计数器            */
	static uint16_t counter_loosen = 0;                                   /* 按键松开计数器            */
	uint8_t key_buffer = 0;                                               /* 按键缓冲值                */
	static uint8_t key_longpress = 0;                                     /* 按键长按标志              */

	key_buffer = XX_Key_Detect();

	if(key_buffer == 23)                                                  /* 组合按键2和3同时长按      */
	{
		counter_press++;
		key_flag = 1;
		if(counter_press>=0x30)
		{
			counter_press = 0;
			key_value = 0;
			return 23;
		}
		return 0;
	}

	if(key_buffer && !key_flag)                                           /* 按键按下                  */
	{
		key_flag = 1;
		key_value = key_buffer;
	}

	if(key_buffer && !key_longpress)                                     /* 按键1、2、3、4长按识别    */
	{
		counter_press++;
		if(counter_press>0x30)
		{
			counter_press = 0;
			key_longpress = 1;
			return key_value*10;
		}
	}

	if(!key_buffer && key_flag)                                         /* 按键按下后松开            */
	{
		key_flag = 0;

		if(key_value && counter_press<=0x30 && !key_longpress)           /* 按键1、2、3、4短按识别    */
		{
			counter_press = 0;
			return key_value;				
		}

		if(key_value && counter_press<=0x30 && key_longpress)
		{
			counter_press = 0;
			key_longpress = 0;				
		}	
	}

	if(!key_flag)                                                      /* 按键松开超过5s             */
	{
		counter_loosen++;
		if(counter_loosen >= 0xff)
		{
			counter_loosen = 0;
			return 5;
		}
	}
	else
		counter_loosen = 0;

	return 0;
}
