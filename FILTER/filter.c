/*
*********************************************************************************************************
* HeadFile
*********************************************************************************************************
*/

#include "filter.h"

/*
*********************************************************************************************************
* 功  能：中位值平均滤波算法
* 形  参：None
* 返回值：ADC值
* 备  注：None
* 作  者：LG
* 日  期：2020/11/15
*********************************************************************************************************
*/

uint16_t Filter_1(void)
{
	uint16_t data[10] = {0};
	uint16_t adc_value = 0;
	uint16_t buffer = 0;	
	uint32_t sum = 0;
	uint8_t i = 0, j = 0;

	for(i=0; i<10; i++)                                                            
	{
		data[i] = Get_Adc();
	}

	for(i=0; i<10-1; i++)        
	{
		for(j=0; j<10-1-i; j++)
		{
			if(data[i]>data[i+1])
			{
				buffer = data[i];
				data[i] = data[i+1];
				data[i+1] = buffer;
			}
		}
	}

	for(i=1; i<9; i++)
	{
		sum += data[i];
	}

	adc_value = sum/8;

	return adc_value;
}

/*
*********************************************************************************************************
* 功  能：递推平均滤波算法
* 形  参：None
* 返回值：ADC值
* 备  注：None
* 作  者：LG
* 日  期：2020/11/15
*********************************************************************************************************
*/

uint16_t Filter_2(uint16_t adc_value)
{
	static uint16_t data[10] = {0};
	static uint8_t i = 0;
	uint8_t j = 0;
	uint32_t sum = 0;
	
	data[i++] = adc_value;
	if(i==10)
		i = 0;
	for(j=0; j<10; j++)
	{
		sum += data[j];
	}
	if(data[9])
		adc_value = sum/10;

	return adc_value;
}

/*
*********************************************************************************************************
* 功  能：消抖滤波算法
* 形  参：None
* 返回值：ADC值
* 备  注：None
* 作  者：LG
* 日  期：2020/11/15
*********************************************************************************************************
*/

uint16_t Filter_3(uint16_t adc_value)
{
	static uint16_t adc_before = 5000;
	static uint8_t counter_b = 0;
	static uint8_t counter_s = 0;

	if(adc_before == 5000)
		adc_before = adc_value;

	if(adc_value > adc_before)
	{
		counter_b++;
		if(counter_b >= 20)
		{
			counter_b = 0;
			adc_before = adc_value;
			return adc_value;
		}
	}
	else
		counter_b = 0;

	if(adc_value < adc_before)
	{
		counter_s++;
		if(counter_s >= 20)
		{
			counter_s = 0;
			adc_before = adc_value;
			return adc_value;
		}
	}
	else
		counter_s = 0;

	return adc_before;
}
