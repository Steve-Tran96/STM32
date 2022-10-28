#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_def.h"

typedef struct
{
  uint8_t NVIC_IRQChannel;                    /*!< Specifies the IRQ channel to be enabled or disabled.
                                                   This parameter can be an enumerator of @ref IRQn_Type 
                                                   enumeration (For the complete STM32 Devices IRQ Channels
                                                   list, please refer to stm32f4xx.h file) */

  uint8_t NVIC_IRQChannelPreemptionPriority;  /*!< Specifies the pre-emption priority for the IRQ channel
                                                   specified in NVIC_IRQChannel. This parameter can be a value
                                                   between 0 and 15 as described in the table @ref MISC_NVIC_Priority_Table
                                                   A lower priority value indicates a higher priority */

  uint8_t NVIC_IRQChannelSubPriority;         /*!< Specifies the subpriority level for the IRQ channel specified
                                                   in NVIC_IRQChannel. This parameter can be a value
                                                   between 0 and 15 as described in the table @ref MISC_NVIC_Priority_Table
                                                   A lower priority value indicates a higher priority */

  uint8_t NVIC_IRQChannelCmd;         /*!< Specifies whether the IRQ channel defined in NVIC_IRQChannel
                                                   will be enabled or disabled. 
                                                   This parameter can be set either to ENABLE or DISABLE */   
} NVIC_InitTypeDef;

void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, uint8_t NewState);
void Set_Priority_EXTI(uint8_t EXTIChannel, uint8_t IRQChannelSub, uint8_t IRQChannelPreemp);
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct);
	
#ifdef __cplusplus
}
#endif

#endif /* __INTERRUPT_H */
