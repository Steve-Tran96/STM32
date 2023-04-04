#ifndef __DMA_H
#define __DMA_H

#include "stm32f4xx_def.h"
#include "stm32f407xx.h"
#include <stdio.h>
#include <stdint.h>

#define DMA2EN						(1U << 22)
#define DMA_SCR_EN				(1U << 0)	
#define DMA_SCR_MINC			(1U << 10)
#define DMA_SCR_PINC			(1U << 9)
#define DMA_SCR_TCIE			(1U << 4)
#define DMA_SCR_TEIE			(1U << 2)
#define DMA_SFCR_DMDIS		(1U << 2)
#define LISR_TCIF0				(1U << 5)
#define LIFCR_CTCIF0			(1U << 5)
#define LISR_TEIF0				(1U << 3)
#define LIFCR_TEIF0				(1U << 3)

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint8_t g_transfer_complete;
	
void dma2_mem2mem_config(void);	
void dma_transfer_start(uint32_t src_buff, uint32_t dest_buff, uint32_t len);
void DMA2_Stream0_IRQHandler(void);
	
	
#ifdef __cplusplus
}
#endif

#endif
	