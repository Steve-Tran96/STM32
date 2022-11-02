#include "spistd.h"


#if defined (STM32F40_41xxx)
	#define USE_SPI1
#endif

#ifdef USE_SPI1
	uint8_t SPI_Buffer[SPI_BUFFER_SIZE];
#endif

#ifdef USE_SPI1
	SPI_t F4x_SPI = {SPI_Buffer, SPI_BUFFER_SIZE, 0, 0, 0};
#endif
	

StatusTypeDef SPI_Enable_CLK(SPI_TypeDef *xpSPI){
	if(xpSPI == SPI1){
		/* Enable SPI1 source Clock */
		RCC->APB2ENR |= (0x1UL << 12); 
	}
	else if(xpSPI == SPI2){
		RCC->APB1ENR |= (0x1UL << 14); 
	}
	else if(xpSPI == SPI3){
		RCC->APB1ENR |= (0x1UL << 14); 
	}
	else return ERROR;
	
	return OK;
}

StatusTypeDef SPI_Init(SPI_Handle *xpSPIHandle){
	/* Enable SPIx source clock */
	SPI_Enable_CLK(xpSPIHandle->pSPIx);
	
	/* Checking SPI mode*/
	if( xpSPIHandle->SPIConfig.SPI_DeviceMode == SPI_MODE_MASTER ){
		/* Select SPI BUS (Full Duplex, Half Duplex, Simplex-TX/RX) */
		if( xpSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_FullDuplex ){
			/* SPI Full Duplex Mode */
			xpSPIHandle->pSPIx->CR1 &= (uint16_t)~(0x01U << 15U);
			
		}else if( xpSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_HalfDuplex ){
			/* SPI Half Duplex Mode */
			xpSPIHandle->pSPIx->CR1 |= (uint16_t)(0x01U << 15U);
			
		}else if( xpSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_Simplex_RX ){
			/* SPI RX Simplex Mode */
			xpSPIHandle->pSPIx->CR1 &= (uint16_t)~( 0x01U << 15U );
			
			xpSPIHandle->pSPIx->CR1 |= (uint16_t)( 0x01U << 10U );
		}	
		/* Define SPI baudrate via select BR[2:0] bits  */
		xpSPIHandle->pSPIx->CR1 |= (uint16_t)( xpSPIHandle->SPIConfig.SPI_CLKSpeed << 3U );
		
		/* Define SPI CPOL, CPHA  */
		xpSPIHandle->pSPIx->CR1 |= (uint16_t)( xpSPIHandle->SPIConfig.SPI_CPOL << 1U );
		xpSPIHandle->pSPIx->CR1 |= (uint16_t)( xpSPIHandle->SPIConfig.SPI_CPHA );
		
		/* Define SPI Data Formart*/
		xpSPIHandle->pSPIx->CR1 |= (uint16_t)( xpSPIHandle->SPIConfig.SPI_DFF << 11U );
		
		xpSPIHandle->pSPIx->CR1 |= (uint16_t)( xpSPIHandle->SPIConfig.SPI_SSM << 9U );
		
		/* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
		xpSPIHandle->pSPIx->I2SCFGR &= 0xF7FF;
		
		/* Write to SPIx CRCPOLY */
		xpSPIHandle->pSPIx->CRCPR = 0x0007;
		
		/* Write mod master */
		xpSPIHandle->pSPIx->CR1 |= (uint16_t)(0x01U << 2U);
		xpSPIHandle->pSPIx->CR1 |= (uint16_t)(0x01U << 8U);
		
		/* Enable SPIx */
		xpSPIHandle->pSPIx->CR1 |= (uint16_t)( 0x01U << 6U);	
		
	}
	else if(xpSPIHandle->SPIConfig.SPI_DeviceMode == SPI_MODE_SLAVE){
		
	}
	else return ERROR;
	
	return OK;
}

void SPI_SendData(SPI_TypeDef *xpSPI, uint16_t *pTxbuffer, uint32_t len){
	
	while(len > 0){
		/* Checking Flag TXE buffer */
		while(!(xpSPI->SR & (uint16_t)(0x01U << 1U)));
		/* Checking DFF */
		if(xpSPI->CR1 & ( 0x01U << 11U )){
			/* 16 bit DFF */
			xpSPI->DR = *((uint16_t*) pTxbuffer); /* Loading data into DR */
			len--;
			len--;
			pTxbuffer++;
		}
		else{
			/* 8 bit DFF */
			xpSPI->DR = *(uint8_t*)(pTxbuffer);
			len--;
			pTxbuffer++;
		}
	}
}

void SPI_ReceiveData(SPI_TypeDef *xpSPI, uint16_t *pTxbuffer, uint32_t len){
	while(len > 0){
		/* Checking Flag RXE buffer */
		while(!(xpSPI->SR & (uint16_t)(0x01U)));
		/* Checking DFF */
		if(xpSPI->CR1 & ( 0x01U << 11U )){
			/* 16 bit DFF */
			*((uint16_t*) pTxbuffer) = (uint16_t)xpSPI->DR; /* Loading data from DR to pTxbuffer address */
			len--;
			len--;
			pTxbuffer++;
		}
		else{
			/* 8 bit DFF */
			*(pTxbuffer) = (uint8_t)xpSPI->DR;
			len--;
			pTxbuffer++;
		}
	}
}

void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data){
  /* Write in the DR register the data to be sent */
  SPIx->DR = Data;
}

uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx)
{
  /* Return the data in the DR register */
  return (uint16_t)SPIx->DR;
}

void SPI_INT_InsertToBuffer(SPI_t* u, uint8_t c) {
	/* Still available space in buffer */
	if (u->Num < u->Size) {
		/* Check overflow */
		if (u->In == u->Size) {
			u->In = 0;
		}
		/* Add to buffer */
		u->Buffer[u->In] = c;
		u->In++;
		u->Num++;
	}
}

SPI_t* SPI_INT_GetUsart(SPI_TypeDef* SPIx) {								
	SPI_t* u;
#ifdef USE_SPI1
	if (SPIx == SPI1) {
		u = &F4x_SPI;
	}else
	{
		return 0;
	}
#endif
	return u;
}

uint8_t SPI_FindCharacter(SPI_TypeDef* SPIx, volatile char c) {
	uint16_t num, out;
	SPI_t* u = SPI_INT_GetUsart(SPIx);
	
	/* Temp variables */
	num = u->Num;
	out = u->Out;
	
	while (num > 0) {
		/* Check overflow */
		if (out == u->Size) {
			out = 0;
		}
		if (u->Buffer[out] == c) {
			/* Character found */
			return 1;
		}
		out++;
		num--;
	}
	/* Character is not in buffer */
	return 0;
}

uint8_t SPI_BufferEmpty(SPI_TypeDef* SPIx){
	SPI_t* u = SPI_INT_GetUsart(SPIx);
	return (u->Num == 0);
}

uint8_t SPI_BufferFull(SPI_TypeDef* SPIx){
	SPI_t* u = SPI_INT_GetUsart(SPIx);
	return (u->Num == u->Size);
}

void SPI_ClearBuffer(SPI_TypeDef* SPIx){
	SPI_t* u = SPI_INT_GetUsart(SPIx);
	u->Num = 0;
	u->In = 0;
	u->Out = 0;
}

void SPI_IT_Enable(SPI_TypeDef* SPIx, uint32_t Flag_IT_TXRX){
	if(Flag_IT_TXRX == Flag_IT_RX){
		/* Enable SPI RX Interrupt */
		SPIx->CR2 |= (0x01U << 6U);
	}else if(Flag_IT_TXRX == Flag_IT_TX){
		/* Enable SPI TX Interrupt */
		SPIx->CR2 |= (0x01U << 7U);
	}
}

void SPI1_IRQHandler(void){
		uint8_t u8spi1;
	/* Checking RXNE Flag, */
	if((SPI1->SR & 0x0001 )!= 0){
		u8spi1 = (uint8_t)SPI1->DR ;
		/* *Note: chi dung 1 function USART_INT_InsertToBuffer trong ngat (neu hon se mat du lieu)  */
		SPI_INT_InsertToBuffer(&F4x_SPI, u8spi1);	
	}	
}

