#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_def.h"

/* define moder of GPIO */
#define GPIO_INPUT		0
#define GPIO_OUTPUT		1 
#define GPIO_ALTFN		2
#define GPIO_ANALOG		3

/* define typer of GPIO */
#define OUTPUT_PP			0
#define OUTPUT_OD			1

/* define PUPD of GPIO */
#define NOPUPD 				0
#define PU 						1
#define PD 						2

/* define Speed of GPIO */
#define LOW_SPEED				1
#define MEDIUM_SPEED		2
#define HIGH_SPEED			3
#define VERYHIGH_SPEED	4

	
typedef struct{
	
	 uint32_t GPIO_Pin;
	 uint32_t GPIO_Mode;
	 uint32_t GPIO_Speed;
	 uint32_t GPIO_PuPdControl;
	 uint32_t GPIO_PinOPType;
	 uint32_t GPIO_PinAltFunMode;
	
}GPIO_Pinconfig;	
	

typedef struct{
	
	GPIO_TypeDef *pGPIOx; /* This holds base address of GPIO port */
	GPIO_Pinconfig GPIO_PinConfig; /* This hold GPIO pin configuration setting*/
	
}GPIO_Handle;
	

StatusTypeDef GPIO_Clock_Cmd(GPIO_TypeDef *pGPIO, uint32_t Cmd);
StatusTypeDef GPIO_DeInit(GPIO_TypeDef *pGPIO);
StatusTypeDef GPIO_Init_Pin(GPIO_Handle *GPIO_Structure);
StatusTypeDef GPIO_Init_Multi(GPIO_Handle *GPIO_Structure);
void GPIO_Write(GPIO_TypeDef* xGPIO, uint8_t Cmd ,uint8_t Pinnum);
void GPIO_Toggle(GPIO_TypeDef* GPIOx, uint16_t Pin);
uint8_t GPIO_Read(GPIO_TypeDef* xGPIO, uint8_t Pinnum);
void GPIO_PinLock(GPIO_TypeDef* GPIOx, uint16_t Pin);

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H */

