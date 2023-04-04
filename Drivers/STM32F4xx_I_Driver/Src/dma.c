#include "dma.h"
#include "Interrupt.h"

volatile uint8_t g_transfer_complete;

void dma2_mem2mem_config(void){
	
	/*Enable clock access DMA module*/
	RCC->AHB1ENR |= DMA2EN;
	/*Disable dma stream*/
	DMA2_Stream0->CR = 0;
	/*Wait until stream is disabled*/
	while(DMA2_Stream0->CR & DMA_SCR_EN);
	/*Configure dma parameter*/
	DMA2_Stream0->CR |= (1<<13); /*Set MSIZE: memory data size (16 bit)*/
	DMA2_Stream0->CR &= ~(0x01UL << 14); 
	
	DMA2_Stream0->CR |= (1<<11); /*Set PSIZE: Peripheral data size (16 bit)*/
	DMA2_Stream0->CR &= ~(0x01UL << 12); 
	
	DMA2_Stream0->CR |= DMA_SCR_MINC; /*Memory increment mode*/
	DMA2_Stream0->CR |= DMA_SCR_PINC; /*Peripheral increment mode*/
	
	DMA2_Stream0->CR &= ~(1U<<6U);	/*Select mem-to-mem transfer*/
	DMA2_Stream0->CR |= (1<<7);			/*Select mem-to-mem transfer*/
	
	DMA2_Stream0->CR |= DMA_SCR_TCIE; /*Enable transfer complete interrupt*/
	DMA2_Stream0->CR |= DMA_SCR_TEIE; /*Enable transfer error interrupt*/
	
	/*Disable direct mode*/
	DMA1_Stream0->FCR |= DMA_SFCR_DMDIS; /*in mode memory-to-memory direct mode must be disable*/
	
	/*Set DMA FIFO threshold*/
	DMA1_Stream0->FCR |= (1U<<0);
	DMA1_Stream0->FCR |= (1U<<1); /*Full FIFO (First In Last Out)*/
	
	/*Enable DMA interrupt in NVIC*/
	NVIC_EnableIRQ(DMA2_Stream0_IRQn); /*core cm4 library*/
	
}

void dma_transfer_start(uint32_t src_buff, uint32_t dest_buff, uint32_t len){
	g_transfer_complete = 0;	
	
	/*Set peripheral address*/
	DMA2_Stream0->PAR = src_buff;
	/*Set memory address*/
	DMA2_Stream0->M0AR = dest_buff; 
	/*Set transfer lenght (number of data register)*/
	DMA2_Stream0->NDTR = len;
	/*Enable dma stream*/
	DMA2_Stream0->CR |= DMA_SCR_EN;
	
	/*Waiting until transfer complete*/
	while(!g_transfer_complete);
	g_transfer_complete = 0;	
}

void DMA2_Stream0_IRQHandler(void){
	
	/*Checking transfer complete event occurred on stream */
	if((DMA2->LISR) & LISR_TCIF0){
		g_transfer_complete = 1;	
		
		/*Clear flag*/
		DMA2->LIFCR |= LIFCR_CTCIF0; /*Write bit 1 to reset*/
	}
	
	/*Checking if transfer error occurred*/
	if((DMA2->LISR) & LISR_TEIF0){
		
		/*Clear flag*/
		DMA2->LIFCR |= LIFCR_TEIF0; /*Write bit 1 to reset*/
	}
}

