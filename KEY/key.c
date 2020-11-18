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

uint8_t key_flag = 0;                                                            /* �����ɿ���־       */

/*
*********************************************************************************************************
* ��  �ܣ�Key��� 
* ��  �Σ�None
* ����ֵ��0�� �ް�������
*		  1�� KEY1�̰�
*		  2�� KEY2�̰�
*		  3�� KEY3�̰�
*		  4�� KEY4�̰�
*		  10��KEY1����
*		  20��KEY2����
*		  30��KEY3����
*		  40��KEY4����
*		  23����ϰ���2��3����
* ��  ע��None
* ��  �ߣ�LG
* ��  �ڣ�2020/11/18
*********************************************************************************************************
*/

uint8_t Key_Detect(void)
{
	static uint8_t key_value = 0;                                         /* ��������ֵ                */
	static uint16_t counter_press = 0;                                    /* �������¼�����            */
	static uint16_t counter_loosen = 0;                                   /* �����ɿ�������            */
	uint8_t key_buffer = 0;                                               /* ��������ֵ                */
	static uint8_t key_longpress = 0;                                     /* ����������־              */

	key_buffer = XX_Key_Detect();

	if(key_buffer == 23)                                                  /* ��ϰ���2��3ͬʱ����      */
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

	if(key_buffer && !key_flag)                                           /* ��������                  */
	{
		key_flag = 1;
		key_value = key_buffer;
	}

	if(key_buffer && !key_longpress)                                     /* ����1��2��3��4����ʶ��    */
	{
		counter_press++;
		if(counter_press>0x30)
		{
			counter_press = 0;
			key_longpress = 1;
			return key_value*10;
		}
	}

	if(!key_buffer && key_flag)                                         /* �������º��ɿ�            */
	{
		key_flag = 0;

		if(key_value && counter_press<=0x30 && !key_longpress)           /* ����1��2��3��4�̰�ʶ��    */
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

	if(!key_flag)                                                      /* �����ɿ�����5s             */
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
