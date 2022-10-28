#include "Interrupt.h"


/* Function Set Priority */
void Set_Priority_EXTI(uint8_t EXTIChannel, uint8_t IRQChannelSub, uint8_t IRQChannelPreemp){
	uint8_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
	/* Compute the Corresponding IRQ Priority --------------------------------*/   
	tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700)) >> 0x08; /* SCB_AIRCR : 0xFA050300 */
  tmppre = (0x04 - tmppriority);
  tmpsub = tmpsub >> tmppriority;

  tmppriority = 	(uint8_t)(IRQChannelSub << tmppre);
  tmppriority |=  (uint8_t)(IRQChannelPreemp & tmpsub);
        
  tmppriority = (uint8_t)(tmppriority << 0x04);
        
  NVIC->IP[EXTIChannel] = tmppriority; 
	
	NVIC->ISER[EXTIChannel >> 0x05U] |= ((uint32_t)0x01 << (EXTIChannel & (uint8_t)0x1F)); /* 0x1U << (6U & 0x1F):(31 line) */
}



