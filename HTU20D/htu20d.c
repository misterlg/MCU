/*
*********************************************************************************************************
* HeadFile
*********************************************************************************************************
*/

#include "htu20d.h"

/*
*********************************************************************************************************
* Global Variable
*********************************************************************************************************
*/

uint8_t sensor_flag = 0;

/*
*********************************************************************************************************
* 功  能：HTU20D初始化
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/11/11
*********************************************************************************************************
*/

void Htu20d_Init(void)
{
	Iic_Pin_Init();
}

/*
*********************************************************************************************************
* 功  能：CRC8校验
* 形  参：*data  - 待校验的数据指针
		  length - 待校验的数据字节数
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/11/11
*********************************************************************************************************
*/

static uint8_t Crc8(uint8_t* data, uint8_t length)
{
	uint8 polynomial = 0x31;                                                     /* 多项式             */
	uint8 crc = 0x00;                                                            /* 初始值             */
	uint8 i = 0, j = 0;

	for(i=0; i<length; i++)
	{
		crc ^= *data++;
		for(j=0; j<8; j++)
		{
			crc = (crc & 0x80) ? (crc << 1) ^ polynomial : (crc << 1);
		}
	}

	return crc;
}

/*
*********************************************************************************************************
* 功  能：HTU20D采集湿度
* 形  参：None
* 返回值：湿度值
* 备  注：非主机模式
* 作  者：LG
* 日  期：2020/11/11
*********************************************************************************************************
*/

float Htu20d_Get_Humidity(void)
{
	static float Humidity = 0;
	uint16_t data = 0;
	uint8_t crc = 0;
	uint8_t cache[2] = {0};

	Iic_Start();                                                                 /* 起始信号           */
	if(!Iic_Send_Byte(HTU20D_ADDRESS_W))		                                 /* 发送设备地址+写    */
	{
		if(!Iic_Send_Byte(HUMIDITY_M))                                           /* 发送湿度测量命令   */
		{
			while(Iic_Send_Byte(HTU20D_ADDRESS_R))                               /* 等待测量完成       */
			{
				Iic_Start();
			}
			cache[0] = Iic_Read_Byte(ACK);                                       /* 读取高字节数据     */
			data = cache[0];
			data <<= 8;
			cache[1] = Iic_Read_Byte(ACK);                                       /* 读取低字节数据     */
			data += cache[1];
			crc = Iic_Read_Byte(NACK);                                           /* 读取CRC校验码      */
			data &= ~(0x3<<0);                                                   /* 清零状态位         */
		}
	}
	Iic_Stop();                                                                  /* 停止信号           */	
	
	if(data == 0)                                                                /* 探头检测           */
	{
		sensor_flag = 1;
		return Humidity;
	}
	else
		sensor_flag = 0;

	if(Crc8(cache,2) == crc)                                                     /* CRC校验            */
		Humidity = -6 + 125 * ((float)data / 65536);                             /* 计算湿度           */
		         
	return Humidity;
}

/*
*********************************************************************************************************
* 功  能：HTU20D采集温度
* 形  参：None
* 返回值：温度值
* 备  注：非主机模式
* 作  者：LG
* 日  期：2020/11/11
*********************************************************************************************************
*/

float Htu20d_Get_Temperature(void)
{
	static float temp = 0;                                      
	uint16_t data = 0;
	uint8_t crc = 0;
	uint8_t cache[2] = {0};

	Iic_Start();                                                                 /* 起始信号           */
	if(!Iic_Send_Byte(HTU20D_ADDRESS_W))		                                 /* 发送设备地址+写    */
	{
		if(!Iic_Send_Byte(TEMPERATURE_M))                                        /* 发送温度测量命令   */
		{
			while(Iic_Send_Byte(HTU20D_ADDRESS_R))                               /* 等待测量完成       */
			{
				Iic_Start();
			}
			cache[0] = Iic_Read_Byte(ACK);                                       /* 读取高字节数据     */
			data = cache[0];
			data <<= 8;
			cache[1] = Iic_Read_Byte(ACK);                                       /* 读取低字节数据     */
			data += cache[1];
			crc = Iic_Read_Byte(NACK);                                           /* 读取CRC校验码      */
			data &= ~(0x3<<0);                                                   /* 清零状态位         */
		}
	}
	Iic_Stop();									                                 /* 停止信号           */

	if(data == 0)                                                                /* 探头检测           */
	{
		sensor_flag = 1;
		return temp;
	}
	else
		sensor_flag = 0;
	
	if(Crc8(cache,2) == crc)                                                     /* CRC校验            */
		temp = -46.85 + 175.72 * ((float)data / 65536) ;                         /* 计算温度           */

	return temp;
}




