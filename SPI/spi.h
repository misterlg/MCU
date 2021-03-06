#ifndef _SPI_H_
#define _SPI_H_

/*
*********************************************************************************************************
* HeadFile
*********************************************************************************************************
*/

#include "XX.h"

/*
*********************************************************************************************************
* Define
*********************************************************************************************************
*/

#define CS_W_1     XX
#define CS_W_0     XX
#define SCK_W_1    XX
#define SCK_W_0    XX
#define SDA_W_1    XX       
#define SDA_W_0    XX

/*
*********************************************************************************************************
* Function
*********************************************************************************************************
*/

void Spi_Init(void);
void Spi_Write(uint8_t data);

#endif

