#define __FILE_INCOMPLETE 1
#include "usart.h"
#include "rcc.h"
#include "Sys.h"
#include "Interrupt.h"
#include "stdio.h"

/**@ERROR: Redefining __FILE (stdio.h) -> Migrate to ARM Compipler 6 (V6)
*@Link: "https://developer.arm.com/documentation/100073/0619/The-Arm-C-and-C---Libraries/
Redefining-low-level-library-functions-to-enable-direct-use-of-high-level-library-functions-in-the-C-library?lang=en"
*/

/*===========================================*/
/* Set USART buffer size  */
#if defined (STM32F40_41xxx)
	#define USE_USART2
#endif

#ifdef USE_USART2
	uint8_t USART_Buffer[USART_BUFFER_SIZE];
#endif

#ifdef USE_USART2
	USART_t F4x_USART = {USART_Buffer, USART_BUFFER_SIZE, 0, 0, 0};
#endif
	
	
void USART_INT_InsertToBuffer(USART_t* u, uint8_t c);
USART_t* USART_INT_GetUsart(USART_TypeDef* USARTx);
uint8_t USART_INT_GetSubPriority(USART_TypeDef* USARTx);
uint8_t USART_BufferFull(USART_TypeDef* USARTx);

/*-------------------- Printf ------------------------*/
	
struct __FILE {
    int handle;
};
 
/* You need this if you want use printf */
/* Struct FILE is implemented in stdio.h */
FILE __stdout;
FILE __stdin;

/* printf() */
int fputc(int c, FILE * f) {
    /* Do your stuff here */
    /* Send your custom byte */
    /* Send byte to USART */
    USART_SendData(USART2, c);   
    /* If everything is OK, you have to return character written */
    return c;
    /* If character is not correct, you can return EOF (-1) to stop writing */
    //return -1;
}

/* scanf() */
int fgetc(FILE * f) {
  /* Do your stuff here */
  /* Send your custom byte */
	/* Send byte to USART */
	char c = USART_Getc(USART2);
  USART_Putc(USART2, c);  /* To echo Received characters back to serial Terminal */
  /* If everything is OK, you have to return character written */
  return c;
  /* If character is not correct, you can return EOF (-1) to stop writing */
  //return -1;
}
/*===========================================*/

/* 
*	Enable or Disable Peripheral Clock USART/UART for STM32F4x
*/
 StatusTypeDef USART_Clock_Control(USART_TypeDef *USART_RegDef, uint8_t EnOrDir){ 
	 if(EnOrDir == ENABLE){
		 	 if(USART_RegDef == USART1){
				 RCC->APB2ENR |= (0x1U << 4U);
			 }
			 else if(USART_RegDef == USART6){
				 RCC->APB2ENR |= (0x1U << 5U);
			 }
			 else if(USART_RegDef == USART2){
				 RCC->APB1ENR |= (0x1U << 17U);
			 }
			 else if(USART_RegDef == USART3){
				 RCC->APB1ENR |= (0x1U << 18U);
			 }
			 else if(USART_RegDef == UART4){
				 RCC->APB1ENR |= (0x1U << 19U);
			 }
			 else if(USART_RegDef == UART5){
				 RCC->APB1ENR |= (0x1U << 20U);
			 }
			 else{
				return ERROR;
			 }
	 }
	 else if(EnOrDir == DISABLE){
		 if(USART_RegDef == USART1){
				 RCC->APB2ENR &= ~(0x1U << 4U);
			 }
			 else if(USART_RegDef == USART6){
				 RCC->APB2ENR &= ~(0x1U << 5U);
			 }
			 else if(USART_RegDef == USART2){
				 RCC->APB1ENR &= ~(0x1U << 17U);
			 }
			 else if(USART_RegDef == USART3){
				 RCC->APB1ENR &= ~(0x1U << 18U);
			 }
			 else if(USART_RegDef == UART4){
				 RCC->APB1ENR &= ~(0x1U << 19U);
			 }
			 else if(USART_RegDef == UART5){
				 RCC->APB1ENR &= ~(0x1U << 20U);
			 }
			 else{
				return ERROR;
			 }	 
	 } 
	 return OK;
}

StatusTypeDef USARTx_DeInit(USART_TypeDef *USART_RegDef){ 
	if(USART_RegDef == USART1){
		RCC->APB2RSTR |= (0x1U << 4U);
		RCC->APB2RSTR &= ~(0x1U << 4U);
	}
	else if(USART_RegDef == USART6){
		RCC->APB2RSTR |= (0x1U << 5U);
		RCC->APB2RSTR &= ~(0x1U << 5U);
	}
	else if(USART_RegDef == USART2){
		RCC->APB1RSTR |= (0x1U << 17U);
		RCC->APB1RSTR &= ~(0x1U << 17U);
	}
	else if(USART_RegDef == USART3){
		RCC->APB1RSTR |= (0x1U << 18U);
		RCC->APB1RSTR &= ~(0x1U << 18U);
	}
	else if(USART_RegDef == UART4){
		RCC->APB1RSTR |= (0x1U << 19U);
		RCC->APB1RSTR &= ~(0x1U << 19U);
	}
	else if(USART_RegDef == UART5){
		RCC->APB1RSTR |= (0x1U << 20U);
		RCC->APB1RSTR &= ~(0x1U << 20U);
	}
	else{
		return ERROR;
	}
	return OK;
}

StatusTypeDef Enable_INT_USART(USART_TypeDef *USART_RegDef){
	if(USART_RegDef == USART1){
		Set_Priority_EXTI(USART1_IRQn, USART_NVIC_PRIORITY , 0x00);
	}
	else if(USART_RegDef == USART6){
		Set_Priority_EXTI(USART6_IRQn, USART_NVIC_PRIORITY , 0x00);
	}
	else if(USART_RegDef == USART2){
		Set_Priority_EXTI(USART2_IRQn, USART_NVIC_PRIORITY , 0x00);
	}
	else if(USART_RegDef == USART3){
		Set_Priority_EXTI(USART3_IRQn, USART_NVIC_PRIORITY , 0x00);
	}
	else if(USART_RegDef == UART4){
		Set_Priority_EXTI(UART4_IRQn, USART_NVIC_PRIORITY , 0x00);
	}
	else if(USART_RegDef == UART5){
		Set_Priority_EXTI(UART5_IRQn, USART_NVIC_PRIORITY , 0x00);
	}
	else{
		return ERROR;
	}
	return OK;
}
 
 StatusTypeDef USART_Init(USART_Handle *pUSARTHandle){ 
	 uint32_t apbclock = 0x00;
	 double usartdiv = 0.0, tmpdiv = 0.0;
	 uint32_t Mantissa = 0;
	 uint8_t Fraction = 0;
	 
	 /*Enable Source Clock for USART*/
	 USART_Clock_Control(pUSARTHandle->xUSART, ENABLE); 
	 /* Disable USART*/
	 pUSARTHandle->xUSART->CR1 &= (uint16_t)~((uint16_t)USART_CR1_UE);	 	 
	 /* Deinit USART2 */
	 USARTx_DeInit(pUSARTHandle->xUSART); 
	  	 
	 /*------------ Clear Bit --------------*/
	 /* Clear M (WordLenght), PCE (Parity), PS (Parity Select), TE(Tranmission) and RE (Reception) bits */
	  pUSARTHandle->xUSART->CR1 &= (uint32_t) ~(uint32_t)((1 << 12) | (1 << 10) | (1 << 9) | (1 << 3) | (1 << 2)); 
	 /* Clear STOP[13:12] bits */
	 pUSARTHandle->xUSART->CR2 &= ~(0x3UL << 12); 
	  /*------------ End Code Clear Bit --------------*/
	 
	 /* Enable USART Tx and Rx engines according to the USART_Mode configuration item */
	 if(pUSARTHandle->USARTConfig.USART_Mode == USART_Mode_TX){
		 pUSARTHandle->xUSART->CR1 |= (0x1U << 3);
	 }
	 else if(pUSARTHandle->USARTConfig.USART_Mode == USART_Mode_RX){
		 pUSARTHandle->xUSART->CR1 |= (0x1U << 2);
	 }
	 else if(pUSARTHandle->USARTConfig.USART_Mode == USART_Mode_TXRX){
		 pUSARTHandle->xUSART->CR1 |= (0x1U << 2) | (0x1U << 3);
	 }
	 
	 /* Implement the code to configure the Word length configuration item */
	 pUSARTHandle->xUSART->CR1 |= (uint16_t)(pUSARTHandle->USARTConfig.USART_WordLenght << 12);	 
	 /* Configuration Parity control bit fields (bit 9, bit 10 in USART_CR1)*/
	 pUSARTHandle->xUSART->CR1 |= (uint16_t)(pUSARTHandle->USARTConfig.USART_ParityControl << 9);	 
	 /* Implement the code to configure the number of STOP bits inserted during USART frame transmission in USART_CR2 bit [12, 13]*/
	 pUSARTHandle->xUSART->CR2 |= (uint16_t)(pUSARTHandle->USARTConfig.USART_NoOfStopBits << 12);
	 
	 /** Configuration of USART hardware flow control bit [8:9] in USART_CR3
	 *@CTS flow control 
	 *@RTS flow control 
	 *@RTS + CTS flow control 
	 */ 
	 pUSARTHandle->xUSART->CR3 |= (uint16_t)(pUSARTHandle->USARTConfig.USART_HWFlowControl << 8);
	 
	 /*Checking xUSART is USART6 Or USART1 (APB1 Source Clock)*/
	if(pUSARTHandle->xUSART == USART6 || pUSARTHandle->xUSART == USART1){
		apbclock = Get_Clock.PCLK2_Freq; /* APB2 Source Clock */
	}
	else{
		apbclock = Get_Clock.PCLK1_Freq; /* APB1 Source Clock */
	}
	
	/* Checking OVER8 bit*/
	if((pUSARTHandle->xUSART->CR1 & (0x1UL << 15)) == 0){
		/* Integer part computing in case Oversampling mode is 8 Samples */
		usartdiv = (double)(apbclock / (double)(16* pUSARTHandle->USARTConfig.USART_Baudrate));	/* double/double = double, double/int = xxxx.000000*/				
	}
	else{
		Mantissa = (uint32_t)usartdiv; /* Get Decimal value */		
		/* Integer part computing in case Oversampling mode is 8 Samples */
		usartdiv = (double)(apbclock / (double)(8* pUSARTHandle->USARTConfig.USART_Baudrate));
	}
	/* Calculating Mantissa */
	/* storing value into template */
	tmpdiv = (usartdiv - (double)Mantissa)*16; /* Converting Fraction to Decimal Value */
	/* Calculating Fraction */
	Fraction = (uint8_t)tmpdiv;	
	 /* Implement the code to the configure baudrate */
	pUSARTHandle->xUSART->BRR |= (Mantissa * 16); /* Add Mantissa into BRR (Mantissa << 4)*/
	pUSARTHandle->xUSART->BRR |= Fraction;	/* Add Mantissa into BRR*/	
	
	/* Enable Usartx */
	pUSARTHandle->xUSART->CR1 |= (0x1UL << 13);
	/* Set bit RX Interrupt USART */
	pUSARTHandle->xUSART->CR1 |= (1 << 5);
	/* Enable interrupt USART*/
	Enable_INT_USART(pUSARTHandle->xUSART);
		
	return OK;
}

StatusTypeDef USART_SendData(USART_TypeDef *pUSARTx , volatile uint8_t Data){
	uint32_t timeout = u32Tick;
	
	while((pUSARTx->SR & (uint16_t)0x0080) == 0){ /* Checking TXE line: if no data transfer to continue*/
		if(u32Tick == (timeout + 5000)){ 
			return TIMEOUT;
		}
	}
	pUSARTx->DR = (Data & (uint8_t)0x01FF);
	
	return OK;
}

StatusTypeDef USART_SendStrData(USART_TypeDef *pUSARTx , uint8_t *pTxBuffer){
	uint32_t timeout = u32Tick;
	char *pdata;
	
	while (*pTxBuffer){
		/* Implement the code to wait until TXE flag is set in the SR*/
		while(!(pUSARTx->SR & (uint16_t)0x0080))
		{
			/* Timeout */
			if(u32Tick == (timeout + 5000)){
				return TIMEOUT;				
			}
		}
		/* Check the USART_WordLength item for 9BIT or 8BIT in a frame */
		if((pUSARTx->CR1 & (uint16_t)0x1000) != 0){
			pdata = (uint8_t*) pTxBuffer;
			pUSARTx->DR |= (*pdata & (uint16_t)0x01FF);
			
			/* check for USART_ParityControl */
			if((pUSARTx->CR1 & 0x0400) == USART_PARITY_NONE)
			{
				/* No parity is used in this transfer. so, 9bits of user data will be sent
				Implement the code to increment pTxBuffer twice */
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				/*Parity bit is used in this transfer . so , 8bits of user data will be sent
				The 9th bit will be replaced by parity bit by the hardware */
				pTxBuffer++;
			}
		}
		else{
			
			pUSARTx->DR = (*pTxBuffer & (uint8_t)0xFF);
			pTxBuffer++;
		}
	}
	return OK;
}

void USART_Puts(USART_TypeDef* USARTx, uint8_t* str) {
	while (*str) {
		/* Send char by uint8_t */
		USART_SendData(USARTx, *str++);
	}
}

 
StatusTypeDef USART_GetData(USART_TypeDef* pUSARTx, char* rxBuffer, uint32_t lenght){
	uint32_t timeout;
	timeout = u32Tick;
	for(uint32_t i = 0 ; i < lenght; i++)
	{
		/* Implement the code to wait until RXNE flag is set in the SR */
		while(!(pUSARTx->SR & (uint16_t)0x20)){ /* Checking RXNE line: if no data transfer to continue*/
		if(u32Tick == (timeout + 5000)){ 
			return 0x01;
			}
		}
		
		/* Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit */
		if((pUSARTx->CR1 & (uint16_t)0x1000) != 0){
			/*We are going to receive 9bit data in a frame*/
			if((pUSARTx->CR1 & 0x0400) == USART_PARITY_NONE){
				/* read only first 9 bits. so, mask the DR with 0x01FF */
				*(rxBuffer) = (char)(pUSARTx->DR  & (uint16_t)0x01FF);
				
				/* increment the pRxBuffer two times */
				rxBuffer++;
				rxBuffer++;
			}
			else{
				/* We are going to receive 8bit data in a frame */
				/* Parity is used, so, 8bits will be of user data and 1 bit is parity */
				 *rxBuffer = (pUSARTx->DR  & (uint8_t)0xFF);
				/* Increment the pRxBuffer */
				*((char*) rxBuffer) = (char)(pUSARTx->DR  & (uint16_t)0x01FF);
			}
		}
		else{
			if((pUSARTx->CR1 & 0x0400) == USART_PARITY_NONE){
				/* 	No parity is used , so all 8bits will be of user data
				*		Read 8 bits from DR
				*/
				*((char*) rxBuffer) = (pUSARTx->DR  & (uint8_t)0xFF);
			}
			else{
				/*	Parity is used, so , 7 bits will be of user data and 1 bit is parity
						Read only 7 bits , hence mask the DR with 0X7F 
				*/			
				*((char*) rxBuffer) = (pUSARTx->DR  & (uint8_t)0x7F); 
			}
			rxBuffer++;
		}
	}

	return OK;
}

uint8_t USART_Getc(USART_TypeDef* USARTx) {
	uint8_t c = 0;
	USART_t* u = USART_INT_GetUsart(USARTx);
	
	/* Check if we have any data in buffer */
	if (u->Num > 0) {
		if (u->Out == u->Size) {
			u->Out = 0;
		}
		c = *(u->Buffer + u->Out);
		u->Out++;
		u->Num--;
	}
	return c;
}

uint16_t USART_Gets(USART_TypeDef* USARTx, uint8_t* buffer, uint16_t bufsize) {
	uint16_t i = 0;
	
	/* Check for any data on USART */
	if (USART_BufferEmpty(USARTx) || (!USART_FindCharacter(USARTx, '\n') && !USART_BufferFull(USARTx))) {
		return 0;
	}
	
	/* If available buffer size is more than 0 characters */
	/* while (i < (bufsize - 1)) */
	while (i < (bufsize)) {
		/* We have available data */
		buffer[i] = (char) USART_Getc(USARTx);
		/* Check for end of string */
		if (buffer[i] == '\n') {
			i++;
			/* Done */
			break;
		} else {
			i++;
		}
	}
	
	/* Add zero to the end of string */
	//buffer[i] = 0;               

	return (i);
}

void USART_INT_InsertToBuffer(USART_t* u, uint8_t c) {
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

uint8_t USART_FindCharacter(USART_TypeDef* USARTx, volatile char c) {
	uint16_t num, out;
	USART_t* u = USART_INT_GetUsart(USARTx);
	
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

USART_t* USART_INT_GetUsart(USART_TypeDef* USARTx) {								
	USART_t* u;
	
#ifdef USE_USART2
	if (USARTx == USART2) {
		u = &F4x_USART;
	}else
	{
		return 0;
	}
#endif
	return u;
}

uint8_t USART_BufferEmpty(USART_TypeDef* USARTx) {
	USART_t* u = USART_INT_GetUsart(USARTx);
	return (u->Num == 0);
}

uint8_t USART_BufferFull(USART_TypeDef* USARTx) {
	USART_t* u = USART_INT_GetUsart(USARTx);
	return (u->Num == u->Size);
}

void USART_ClearBuffer(USART_TypeDef* USARTx) {
	USART_t* u = USART_INT_GetUsart(USARTx);
	
	u->Num = 0;
	u->In = 0;
	u->Out = 0;
}

/* Processing IRQ_USART2 */
void USART2_IRQHandler(void){
	uint8_t u8charUsart2;
	/* Checking RXNE Flag, */
	if((USART2->SR & 0x0020 )!= 0){
		u8charUsart2 = (uint8_t)USART2->DR ;
		/* *Note: chi dung 1 function USART_INT_InsertToBuffer trong ngat (neu hon se mat du lieu)  */
		USART_INT_InsertToBuffer(&F4x_USART, u8charUsart2);	
	}	
}




