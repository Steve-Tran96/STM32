#ifndef __SPI_H
#define __SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_def.h"

typedef struct
{
  uint16_t SPI_DeviceMode;
	uint16_t SPI_BusConfig;
	uint16_t SPI_CLKSpeed;
	uint16_t SPI_DFF;
	uint16_t SPI_CPOL;
	uint16_t SPI_CPHA;
	uint16_t SPI_SSM;
	
}SPI_Config;


typedef struct{
	
	SPI_TypeDef *pSPIx;
	SPI_Config SPIConfig;
	
}SPI_Handle;


typedef struct {
	uint8_t *Buffer;
	uint16_t Size;
	uint16_t Num;
	uint16_t In;
	uint16_t Out;
} SPI_t;

#define SPI_BUFFER_SIZE 16
extern uint8_t SPI_Buffer[SPI_BUFFER_SIZE];
extern SPI_t F4x_SPI;

/**
@SPI: Device Mode 
*/
#define SPI_MODE_SLAVE	 			0x00U
#define SPI_MODE_MASTER 			0x01U

/**
@SPI: Bus Config
*/
#define SPI_BUS_FullDuplex		0x00U
#define SPI_BUS_HalfDuplex		0x02U
#define SPI_BUS_Simplex_TX		0x03U
#define SPI_BUS_Simplex_RX		0x04U 


/**
@SPI: Clock Speed (Baudrate)
*/
#define SPI_CLK_DIV2					0x00U
#define SPI_CLK_DIV4					0x01U
#define SPI_CLK_DIV8					0x02U
#define SPI_CLK_DIV16					0x03U
#define SPI_CLK_DIV32					0x04U
#define SPI_CLK_DIV64					0x05U
#define SPI_CLK_DIV128				0x06U
#define SPI_CLK_DIV256				0x07U

/**
@SPI: Data Format
*/
#define SPI_DFF_8BIT					0x00U 
#define SPI_DFF_16BIT					0x01U

/**
@SPI: CPOL
*/
#define SPI_CPOL_L						0x00U
#define SPI_CPOL_H						0x01U

/**
@SPI: CPHA
*/
#define SPI_CPHA_1EDGE				0x00U
#define SPI_CPHA_2EDGE				0x01U

/**
@SPI: SSM
*/
#define SPI_SSM_HARDWARE			0x00U
#define SPI_SSM_SOFTWARE			0x01U

/**
*@SPI: application state
*/
#define SPI_READY							0x00U
#define SPI_BUSY_IN_TX				0x01U
#define SPI_BUSY_IN_RX				0x02U

/**
*@Flag_IT_TXRX
*/

#define Flag_IT_TX ( 0x01U << 7U )
#define Flag_IT_RX ( 0x01U << 6U )

StatusTypeDef SPI_Enable_CLK(SPI_TypeDef *xpSPI);
StatusTypeDef SPI_Init(SPI_Handle *xpSPIHandle);
void SPI_SendData(SPI_TypeDef *xpSPI, uint16_t *pTxbuffer, uint32_t len);
void SPI_ReceiveData(SPI_TypeDef *xpSPI, uint16_t *pTxbuffer, uint32_t len);
void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data);
uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx);

void SPI_INT_InsertToBuffer(SPI_t* u, uint8_t c);
uint8_t SPI_FindCharacter(SPI_TypeDef* SPIx, volatile char c);
SPI_t* SPI_INT_GetUsart(SPI_TypeDef* SPIx);

uint8_t SPI_BufferEmpty(SPI_TypeDef* SPIx);
uint8_t SPI_BufferFull(SPI_TypeDef* SPIx);
void SPI_ClearBuffer(SPI_TypeDef* SPIx);
void SPI_ITConfig(SPI_TypeDef* SPIx);
void SPI_IT_Enable(SPI_TypeDef* SPIx, uint32_t Flag_IT_TXRX);
void SPI1_IRQHandler(void);
	
#ifdef __cplusplus
}
#endif

#endif /* __SPI_H */
