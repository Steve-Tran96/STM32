#include "main.h"

#define BUFFER_SIZE 	10

uint16_t sensor_data_arr[BUFFER_SIZE] = {892, 731, 1234, 90, 25, 23, 124, 665, 881, 999};
uint16_t temp_data_arr[BUFFER_SIZE];

void config_GPIOD(void);
void config_USART(void);
void Initialize(void);

int main(){
	/*Initializing System*/
	Initialize();
	/*Initializing USART2*/
	config_USART();
	config_GPIOD();
	Delay_ms(250);
	
	dma2_mem2mem_config();
	dma_transfer_start((uint32_t)sensor_data_arr, (uint32_t)temp_data_arr, BUFFER_SIZE);
	printf("\nInitializing OK\n Let's Go!\n");
	
	uint8_t x = 0;
	
	while(1){
		GPIO_Toggle(GPIOD, PIN12);
		Delay_ms(100);	
		GPIO_Toggle(GPIOD, PIN13);
		Delay_ms(100);
		GPIO_Toggle(GPIOD, PIN14);
		Delay_ms(100);
		GPIO_Toggle(GPIOD, PIN15);
		Delay_ms(100);
		
		
		if(x== 10){
			for(int i=0; i<BUFFER_SIZE; i++){
				printf("\nTemp buffer[%d]: %d\n",i, temp_data_arr[i]);
			}
			x=0;
		}
		x++;
		
	}
}

/**---------------------------------------**/

void Initialize(void){
	if(SystemClock_Config_HSE() != OK){ /* Init clock for system */
		__disable_irq();
		while (1){
			/* LOOP */
		}
	}
	Init_SysTick(); /* Init Systick for system */
	uint32_t tempPriority;
	tempPriority =__NVIC_GetPriority(SysTick_IRQn);
}

void config_GPIOD(void){
	GPIO_Handle GPIO_StructInit;
	/* Configuration Output LED pin */
	GPIO_StructInit.pGPIOx = GPIOD;
	GPIO_StructInit.GPIO_PinConfig.GPIO_Pin = ( PIN12 | PIN13 | PIN14 | PIN15 );
	GPIO_StructInit.GPIO_PinConfig.GPIO_Mode = 0x01UL; /* Output mode*/
	GPIO_StructInit.GPIO_PinConfig.GPIO_PinOPType = 0x0UL; /* Output Push - Pull*/
	GPIO_StructInit.GPIO_PinConfig.GPIO_Speed = 0x03UL; /* Very High Speed*/
	GPIO_StructInit.GPIO_PinConfig.GPIO_PuPdControl = 0x01UL; /* Pull Up mode */
	GPIO_Init_Multi(&GPIO_StructInit);
}

void config_USART(void){
	
	USART_Handle xpUSART;
	GPIO_Handle GPIO_USARTx;
	
	/* Configuration USART pin */
	GPIO_USARTx.pGPIOx = GPIOD;
	GPIO_USARTx.GPIO_PinConfig.GPIO_Pin = ( PIN5 | PIN6 ); /* PD5: USART2_TX - RX (Peripheral), PD6: USART2_RX - TX (Peripheral) */
	GPIO_USARTx.GPIO_PinConfig.GPIO_Mode = 0x02UL; /* AF mode*/
	GPIO_USARTx.GPIO_PinConfig.GPIO_PinOPType = 0x0UL; /* Output Push - Pull*/
	GPIO_USARTx.GPIO_PinConfig.GPIO_Speed = 0x03UL; /* Very High Speed*/
	GPIO_USARTx.GPIO_PinConfig.GPIO_PuPdControl = 0x00UL; /* PUPD mode */
	GPIO_USARTx.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_AF_USART2;
	GPIO_Init_Multi(&GPIO_USARTx);
	
	xpUSART.xUSART = USART2;
	xpUSART.USARTConfig.USART_Baudrate = USART_Baudrate_9600;
	xpUSART.USARTConfig.USART_HWFlowControl = 0;
	xpUSART.USARTConfig.USART_Mode = USART_Mode_TXRX;
	xpUSART.USARTConfig.USART_NoOfStopBits = 0; 	/* 0: 1 Stop bit */
	xpUSART.USARTConfig.USART_ParityControl = USART_PARITY_NONE; 	/* Disable Partity control*/
	xpUSART.USARTConfig.USART_WordLenght = 0; 		/* 8 bit, 1 Start bit*/
	
	USART_Init(&xpUSART);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
