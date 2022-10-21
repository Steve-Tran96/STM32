#ifndef __SPI_H
#define __SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_def.h"

typedef struct
{
  uint32_t SPI_DeviceMode;
	uint32_t SPI_BusConfig;
	uint32_t SPI_CLKSpeed;
	uint32_t SPI_DFF;
	uint32_t SPI_CPOL;
	uint32_t SPI_CPHA;
	uint32_t SPI_SSM;
	
} SPI_Config;


typedef struct{
	
	SPI_TypeDef *pSPIx;
	SPI_Config SPIConfig;
	
}SPI_Handle;


#ifdef __cplusplus
}
#endif

#endif /* __SPI_H */
