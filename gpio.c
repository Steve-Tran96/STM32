#include "gpio.h"

StatusTypeDef GPIO_Clock_Cmd(GPIO_TypeDef *pGPIO, uint32_t Cmd){
	
	if(Cmd == ENABLE){
		if(pGPIO == GPIOA){
			CLK_GPIOA_EN();
		}else if(pGPIO == GPIOB){
			CLK_GPIOB_EN();
		}else if(pGPIO == GPIOC){
			CLK_GPIOC_EN();
		}else if(pGPIO == GPIOD){
			CLK_GPIOD_EN();
		}else if(pGPIO == GPIOE){
			CLK_GPIOE_EN();
		}else if(pGPIO == GPIOF){
			CLK_GPIOF_EN();
		}else if(pGPIO == GPIOG){
			CLK_GPIOG_EN();
		}else if(pGPIO == GPIOH){
			CLK_GPIOH_EN();
		}else if(pGPIO == GPIOI){
			CLK_GPIOI_EN();
		}
	}else{
		/* TODO */
	}
	
	return OK;
}

StatusTypeDef GPIO_DeInit(GPIO_TypeDef *pGPIO){
	if(pGPIO == GPIOA){
		CLK_GPIOA_DI();
	}else if(pGPIO == GPIOB){
		CLK_GPIOB_DI();
	}else if(pGPIO == GPIOC){
		CLK_GPIOC_DI();
	}else if(pGPIO == GPIOD){
		CLK_GPIOD_DI();
	}else if(pGPIO == GPIOE){
		CLK_GPIOE_DI();
	}else if(pGPIO == GPIOF){
		CLK_GPIOF_DI();
	}else if(pGPIO == GPIOG){
		CLK_GPIOG_DI();
	}else if(pGPIO == GPIOH){
		CLK_GPIOH_DI();
	}else if(pGPIO == GPIOI){
		CLK_GPIOI_DI();
	}
	return OK;
}

/* Configurating "one" pin */
StatusTypeDef GPIO_Init_Pin(GPIO_Handle *GPIO_Structure){
	/* Checking Pin number <= 15 [0:15]*/
	if(GPIO_Structure->GPIO_PinConfig.GPIO_Pin < 16){
		/* Deinit GPIO */
			GPIO_Clock_Cmd(GPIO_Structure->pGPIOx, DISABLE);
			GPIO_Structure->pGPIOx->MODER 	&= ~(0x3U << GPIO_Structure->GPIO_PinConfig.GPIO_Pin*2);
			GPIO_Structure->pGPIOx->OTYPER 	&= ~(0x1U << GPIO_Structure->GPIO_PinConfig.GPIO_Pin);
			GPIO_Structure->pGPIOx->OSPEEDR &= ~(0x3U << GPIO_Structure->GPIO_PinConfig.GPIO_Pin*2);
			GPIO_Structure->pGPIOx->PUPDR 	&= ~(0x3U << GPIO_Structure->GPIO_PinConfig.GPIO_Pin*2);
			
			/* checking and configuration MODER[0:3] */
			if(GPIO_Structure->GPIO_PinConfig.GPIO_Mode <= GPIO_ANALOG){
				GPIO_Structure->pGPIOx->MODER |= (GPIO_Structure->GPIO_PinConfig.GPIO_Mode << (GPIO_Structure->GPIO_PinConfig.GPIO_Pin*2));
				
				if(GPIO_Structure->GPIO_PinConfig.GPIO_Mode == GPIO_OUTPUT){ /* checking MODE is OUTPUT?*/			
					/* checking and configuration OTYPER[0:1] */
					if(GPIO_Structure->GPIO_PinConfig.GPIO_PinOPType < 4){
						GPIO_Structure->pGPIOx->OTYPER |= (GPIO_Structure->GPIO_PinConfig.GPIO_PinOPType << GPIO_Structure->GPIO_PinConfig.GPIO_Pin);
					}
					else{
						return ERROR;
					}
				}
				/* checking and configuration OSPEED */
				
			}
			else{
				return ERROR;
			}
			
			/* checking and configuration SPEED[0:3] */
			if(GPIO_Structure->GPIO_PinConfig.GPIO_Mode < 4){
				GPIO_Structure->pGPIOx->OSPEEDR |= (GPIO_Structure->GPIO_PinConfig.GPIO_Speed << (GPIO_Structure->GPIO_PinConfig.GPIO_Pin*2));
			}
			else{
				return ERROR;
			}	
			
			/* checking and configuration PUPD[0:3] */
			if(GPIO_Structure->GPIO_PinConfig.GPIO_Mode < 4){
				GPIO_Structure->pGPIOx->PUPDR |= (GPIO_Structure->GPIO_PinConfig.GPIO_PuPdControl << (GPIO_Structure->GPIO_PinConfig.GPIO_Pin*2));
			}
			else{
				return ERROR;
			}
	}
	else{
		return ERROR;
	}

	/* Enable Clock for GPIO */
	GPIO_Clock_Cmd(GPIO_Structure->pGPIOx, ENABLE);
	
	return OK;
}

/* Configurating some pin at the time */
StatusTypeDef GPIO_Init_Multi(GPIO_Handle *GPIO_Structure){
	uint32_t pin = 0, currentpin = 0;
	if(GPIO_Structure->GPIO_PinConfig.GPIO_Pin <= 0xffffUL) { /* Checking pin < max pin*/
		/* Enable Clock for GPIO */
		GPIO_Clock_Cmd(GPIO_Structure->pGPIOx, ENABLE);
		for(int pos = 0; pos < 16; pos++){
			pin = (1 << pos);
			currentpin = ( GPIO_Structure->GPIO_PinConfig.GPIO_Pin & pin );
			if(currentpin == pin){
					GPIO_Structure->pGPIOx->MODER &= ~(0x3UL << (pos*2));
					GPIO_Structure->pGPIOx->MODER |= 	(GPIO_Structure->GPIO_PinConfig.GPIO_Mode << (pos*2));
				
					if(GPIO_Structure->GPIO_PinConfig.GPIO_Mode == 0x1U || GPIO_Structure->GPIO_PinConfig.GPIO_Mode == 0x2U){
						GPIO_Structure->pGPIOx->OTYPER &= ~(0x1UL << (pos*2));
						GPIO_Structure->pGPIOx->OTYPER |= ( GPIO_Structure->GPIO_PinConfig.GPIO_PinOPType << pos*2 );
						GPIO_Structure->pGPIOx->OSPEEDR &= ~(0x3UL << (pos*2));
						GPIO_Structure->pGPIOx->OSPEEDR |= ( GPIO_Structure->GPIO_PinConfig.GPIO_Speed << pos*2 );					
					}
					
					GPIO_Structure->pGPIOx->PUPDR &= ~(0x3UL << (pos*2));
					GPIO_Structure->pGPIOx->PUPDR |= ( GPIO_Structure->GPIO_PinConfig.GPIO_PuPdControl << pos*2 );
			}
		}	
	}
	else{
		return ERROR;
	}
	
	return OK;
}


void GPIO_Write(GPIO_TypeDef* xGPIO, uint8_t Cmd ,uint8_t Pinnum){
	
	if(Cmd == SET){
		xGPIO->ODR = (0x1U << Pinnum);
	}
}

/**
  * @brief  Toggles the specified GPIO pins..
  * @param  GPIOx: where x can be (A..K) to select the GPIO peripheral for STM32F405xx/407xx and STM32F415xx/417xx devices
  *                      x can be (A..I) to select the GPIO peripheral for STM32F42xxx/43xxx devices.
  *                      x can be (A, B, C, D and H) to select the GPIO peripheral for STM32F401xx devices. 
  * @param  GPIO_Pin: Specifies the pins to be toggled.
  * @retval None
  */

void GPIO_Toggle(GPIO_TypeDef* GPIOx, uint16_t Pin){
	
  GPIOx->ODR ^= (0x1U << Pin);
}

/**
  * @brief  Reads the specified output data port bit.
  * @param  GPIOx: where x can be (A..K) to select the GPIO peripheral for STM32F405xx/407xx and STM32F415xx/417xx devices
  *                      x can be (A..I) to select the GPIO peripheral for STM32F42xxx/43xxx devices.
  *                      x can be (A, B, C, D and H) to select the GPIO peripheral for STM32F401xx devices. 
  * @param  GPIO_Pin: specifies the port bit to read.
  *          This parameter can be GPIO_Pin_x where x can be (0..15).
  * @retval The output port pin value.
  */
uint8_t GPIO_Read(GPIO_TypeDef* xGPIO, uint8_t Pinnum){
	uint8_t u8Value;
	
	if((xGPIO->IDR & (1 << Pinnum)) != RESET) {
		u8Value = (uint8_t)SET;
	}
	else{
		u8Value = (uint8_t)RESET;
	}
	
	return u8Value;
}

/**
  * @brief  Locks GPIO Pins configuration registers.
  * @note   The locked registers are GPIOx_MODER, GPIOx_OTYPER, GPIOx_OSPEEDR,
  *         GPIOx_PUPDR, GPIOx_AFRL and GPIOx_AFRH.
  * @note   The configuration of the locked GPIO pins can no longer be modified
  *         until the next reset.
  * @param  GPIOx: where x can be (A..K) to select the GPIO peripheral for STM32F405xx/407xx and STM32F415xx/417xx devices
  *                      x can be (A..I) to select the GPIO peripheral for STM32F42xxx/43xxx devices.
  *                      x can be (A, B, C, D and H) to select the GPIO peripheral for STM32F401xx devices. 
  * @param  GPIO_Pin: specifies the port bit to be locked.
  *          This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval None
  */
void GPIO_PinLock(GPIO_TypeDef* GPIOx, uint16_t Pin){
	
	GPIOx->LCKR |= ((0x1UL << 16) | 0x1U << Pin);
	
}
