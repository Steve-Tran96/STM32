#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_def.h"

/* Configuration structure for USARTx peripheral */
typedef struct{
	uint32_t 	USART_Mode;
	uint32_t 	USART_Baudrate; 		/* more than 65535 (16 bits) */
	uint32_t 	USART_NoOfStopBits;
	uint32_t 	USART_WordLenght;
	uint32_t 	USART_ParityControl;
	uint32_t 	USART_HWFlowControl;
}USART_Config_t;

/*
* @USART Mode
*/
#define USART_Mode_TX 		0
#define USART_Mode_RX 		1
#define USART_Mode_TXRX 	2

/**
* @USART Baudrate
* @Baudrate = fCK/(8 * ( 2 - OVER8 ) * USARTDIV )
* @Baudrate (smartcard) = fCK/( 16 * USARTDIV )
* 
*/
#define USART_Baudrate_1200				1200
#define USART_Baudrate_2400				2400
#define USART_Baudrate_9600				9600
#define USART_Baudrate_19200			19200
#define USART_Baudrate_38400			38400
#define USART_Baudrate_57600			57600
#define USART_Baudrate_115200			115200
#define USART_Baudrate_230000			230400
#define USART_Baudrate_460000			460800
#define USART_Baudrate_921000			921600
#define USART_Baudrate_2M					2000000
#define USART_Baudrate_3M					3000000


/**
*@Word Lenght
*/
#define USART_8Bits 	0
#define USART_9Bits		1

/**
*@Parity control 
*/
#define USART_PARITY_ODD 		3
#define USART_PARITY_EVN 		2
#define USART_PARITY_NONE 	0

/**
*@USART NoOfStopBits
*/
#define USART_STOPBIT_1			0
#define USART_STOPBIT_0_5		1
#define USART_STOPBIT_2			2
#define USART_STOPBIT_1_5		3

/*
 *@USART_HWFlowControl
 *Possible options for USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_RTS    	1
#define USART_HW_FLOW_CTRL_CTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3

	
/* Handle structure  for USARTx peripheral */
typedef struct{
	
	USART_TypeDef *xUSART;
	USART_Config_t USARTConfig;
	
}USART_Handle;

/* -----Internal Buffer USART -----------*/
typedef struct {
	uint8_t *Buffer;
	uint16_t Size;
	uint16_t Num;
	uint16_t In;
	uint16_t Out;
} USART_t;

#ifndef USART_BUFFER_SIZE
	#define USART_BUFFER_SIZE 				32
#endif

/* NVIC Priority */
#ifndef USART_NVIC_PRIORITY
	#define USART_NVIC_PRIORITY				0x00
#endif


extern uint8_t USART_Buffer[USART_BUFFER_SIZE];
extern USART_t F4x_USART;

/* Functions to Init USART */
StatusTypeDef USART_Clock_Control(USART_TypeDef *USART_RegDef, uint8_t EnOrDir);
StatusTypeDef USARTx_DeInit(USART_TypeDef *USART_RegDef);
StatusTypeDef Enable_INT_USART(USART_TypeDef *USART_RegDef);
StatusTypeDef USART_Init(USART_Handle *pUSARTHandle);

/* Functions to Send USART Data */
StatusTypeDef USART_SendData(USART_TypeDef *pUSARTHandle , volatile uint8_t Data);
StatusTypeDef USART_SendStrData(USART_TypeDef *pUSARTx , uint8_t *pTxBuffer);
void USART_Puts(USART_TypeDef* USARTx, uint8_t* str);

/* Functions to Receive USART Data */
StatusTypeDef USART_GetData(USART_TypeDef* pUSARTx, char* rxBuffer, uint32_t lenght);
uint8_t USART_Getc(USART_TypeDef* USARTx);
uint16_t USART_Gets(USART_TypeDef* USARTx, uint8_t* buffer, uint16_t bufsize);

/* Functions to Processing USART Data */
uint8_t USART_BufferEmpty(USART_TypeDef* USARTx);
uint8_t USART_BufferFull(USART_TypeDef* USARTx);
void USART_ClearBuffer(USART_TypeDef* USARTx);
uint8_t USART_FindCharacter(USART_TypeDef* USARTx, volatile char c);

/* Interrupt USART Function */
void USART2_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __USART_H */
