#include "exti.h"
/**
@SYSCFG_EXTICR1: EXTI[0:3]
@SYSCFG_EXTICR2: EXTI[4:7]
@SYSCFG_EXTICR3: EXTI[8:11]
@SYSCFG_EXTICR4: EXTI[12:15]
*/

StatusTypeDef EXTI_Init(uint8_t EXTIChannel, uint8_t EXTIline, uint8_t Trigger){
	
	/* Enable system configuration controller clock enable */
	RCC->APB2ENR |= (0x1UL << 14U);
	/*Configuration EXTI follow pin [0..15]*/
	SYSCFG->EXTICR[((1 << EXTIline) >> 0x02)] |= 0x00; /* Select  EXTI[Channel] - PA0*/
	
	/*Setup Interrupt */
	/*Clear Pending Request*/
	EXTI->PR |= (0x1UL << EXTIline); /* This bit is cleared by programming it to ‘1’ */
	
	if(Trigger == RAISING){
		/*Configuration Raising trigger*/
		EXTI->RTSR = (0x1UL << EXTIline);
	}else if(Trigger == FALLING){
		/*Configuration Falling trigger*/
		EXTI->FTSR |= (0x1UL << EXTIline); /*Enable Falling trigger 0 [0:22]*/
	}else if((Trigger == RAISING)&&(Trigger == FALLING)){
		/*Configuration Falling trigger*/
		EXTI->FTSR |= (0x1UL << EXTIline); /*Enable Falling trigger 0 [0:22]*/
		/*Configuration Raising trigger*/
		EXTI->RTSR = 0x0UL; /*Disable all trigger*/
	}
	else{
		return ERROR;
	}

	/*Configuration Software interrupt event*/
	EXTI->SWIER |= (0x1UL << EXTIline);
	/*Configuration Interrupt mask register*/
	EXTI->IMR |= (0x1UL << EXTIline);
	
	/*Enable IRQ Channels [EXTIChannel]*/
	/* Set Priority for Extension Interrrupt */
	//Set_Priority_EXTI(EXTIChannel, (uint8_t)(1 << EXTIline), (EXTIChannel + 7U)); /* Priority = Position + 7U */
	Set_Priority_EXTI(EXTIChannel, 0x0A, (uint8_t)(1 << EXTIline));
	return OK;
}


