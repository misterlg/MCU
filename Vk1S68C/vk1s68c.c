/*
*********************************************************************************************************
* HeadFile
*********************************************************************************************************
*/

#include "vk1s68c.h"

/*
*********************************************************************************************************
* 功  能：Vk1s68c引脚初始化
* 形  参：None
* 返回值：None
* 备  注：STB - XX - 推挽输出 
*	      CLK - XX - 推挽输出 
*	      DIO - XX - 开漏输出 
* 作  者：LG
* 日  期：2020/11/13
*********************************************************************************************************
*/

static void Vk1s68c_Pin_Init(void)
{
	XX
}

/*
*********************************************************************************************************
* 功  能：Vk1s68c写数据
* 形  参：data - 待写入的数据
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/11/13
*********************************************************************************************************
*/

static void Vk1s68c_Write_Data(uint8_t data)
{
	uint8_t i = 0;
	
	for(i=0; i<8; i++)
	{
		CLK_W_0;
		if(data & 0x01)
			DIO_W_1;
		else
			DIO_W_0;
		data >>= 1;
		CLK_W_1;
	}
}

/*
*********************************************************************************************************
* 功  能：Vk1s68c读数据
* 形  参：None
* 返回值：读取的数据
* 备  注：None
* 作  者：LG
* 日  期：2020/11/13
*********************************************************************************************************
*/

static uint8_t Vk1s68c_Read_Data(void)
{
	uint8_t i = 0;
	uint8_t data = 0;

	DIO_W_1;                                                                 /* 切换为输入         */

	for(i=0; i<8; i++)
	{ 
	    __asm
		{
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
		}
		data >>= 1;
		CLK_W_0;
		__asm
		{
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
		}
		if(DIO_R == 1)
			data |= 0x80;
		CLK_W_1;
	}
	
	return data;
}

/*
*********************************************************************************************************
* 功  能：Vk1s68c写命令
* 形  参：cmd - 待写入的命令
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/11/13
*********************************************************************************************************
*/

static void Vk1s68c_Write_Cmd(uint8_t cmd)
{
	STB_W_0;
	Vk1s68c_Write_Data(cmd);
	STB_W_1;
}

/*
*********************************************************************************************************
* 功  能：Vk1s68c指定地址写入数据
* 形  参：address - 地址
*		  data - 待写入数据
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/11/13
*********************************************************************************************************
*/

void Vk1s68c_Write_Data_Address(uint8_t address, uint8_t data)
{
	STB_W_0;
	Vk1s68c_Write_Data(0xc0 | address);
	Vk1s68c_Write_Data(data);
	STB_W_1;
}

/*
*********************************************************************************************************
* 功  能：Vk1s68c初始化
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/11/13
*********************************************************************************************************
*/

void Vk1s68c_Init(void)
{
	uint8_t i = 0;

	Vk1s68c_Pin_Init();
	Vk1s68c_Write_Cmd(0x44);
	Vk1s68c_Write_Cmd(0x8f);

	for(i=0; i<14; i++)
	{
		Vk1s68c_Write_Data_Address(i, 0x00);
	}
}

/*
*********************************************************************************************************
* 功  能：Vk1s68c数码管刷新 
* 形  参：*number - 数据指针
* 返回值：None
* 备  注：共阴接法 
* 作  者：LG
* 日  期：2020/11/13
*********************************************************************************************************
*/

void Vk1s68c_Digital_Refresh(uint8_t* number)
{
	uint8_t i = 0;

	for(i=0; i<7; i++)
	{
		Vk1s68c_Write_Data_Address(i*2, number[i]);
	}
}

/*
*********************************************************************************************************
* 功  能：Vk1s68c按键检测 
* 形  参：None
* 返回值：按键按下值
* 备  注：None
* 作  者：LG
* 日  期：2020/11/13
*********************************************************************************************************
*/

uint8_t Vk1s68c_Key_Detect(void)
{
	uint8_t buff[2] = {0};
	uint8_t i = 0;
	uint8_t key_value = 0;

	STB_W_0;
	Vk1s68c_Write_Data(0x42);                                                   /* 读键扫数据命令     */
	for(i=0; i<2; i++)
		buff[i] = Vk1s68c_Read_Data();
	STB_W_1;
	
	if(buff[0] & (0x01<<0))
		key_value = 1;
	if(buff[0] & (0x01<<3))
		key_value = 2;
	if(buff[1] & (0x01<<0))
		key_value = 3;
	if(buff[1] & (0x01<<3))
		key_value = 4;
	if((buff[0] & (0x01<<3)) && (buff[1] & (0x01<<0)))
		key_value = 23;

	if((buff[0] & (0x01<<0)) && (buff[1] & (0x01<<0)))
		key_value = 13;

	if((buff[0] & (0x01<<3)) && (buff[1] & (0x01<<3)))
		key_value = 24;

	return key_value;
}

