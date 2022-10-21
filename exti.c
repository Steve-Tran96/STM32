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
	SYSCFG->EXTICR[0] |= 0x00; /* Select  EXTI[Channel] - PA0*/
	
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
	NVIC->ISER[EXTIChannel >> 0x05U] |= ((uint32_t)0x1UL << (EXTIChannel & (uint8_t)0x1F)); /* 0x1U << (6U & 0x1F):(31 line) */
	
	//NVIC->IP[EXTIChannel] = 13U << 0x04;
	Set_Priority_EXTI(EXTIChannel, EXTIline, (EXTIChannel + 7U)); /* Priority = Position + 7U */
	return OK;
}

/* Function Set Priority for Extension Interrrupt */
void Set_Priority_EXTI(uint8_t EXTIChannel, uint8_t EXTIline, uint8_t Priority){
	uint8_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
	/* Compute the Corresponding IRQ Priority --------------------------------*/   
	tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700)) >> 0x08; /* SCB_AIRCR : 0xFA050300 */
  tmppre = (0x4 - tmppriority);
  tmpsub = tmpsub >> tmppriority;

  tmppriority = 	Priority << tmppre;
  tmppriority |=  (uint8_t)(EXTIline & tmpsub);
        
  tmppriority = tmppriority << (uint8_t)0x04;
        
  NVIC->IP[EXTIChannel] = tmppriority; 
}



