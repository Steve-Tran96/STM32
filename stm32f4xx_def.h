/**
  ******************************************************************************
  * @file    stm32f4xx_def.h
  * @author  MCD Application Team
  * @brief   This file contains HAL common defines, enumeration, macros and 
  *          structures definitions. 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_DEF_H
#define __STM32F4xx_DEF_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f407xx.h"

/* define some generic macros */

#define ENABLE 		1
#define DISABLE 	0
#define SET 			ENABLE
#define RESET 		DISABLE
#define ON				SET
#define OFF				RESET

/* Enable Clock some GPIO on AHB bus */
#define CLK_GPIOA_EN() ( RCC->AHB1ENR |= (0x1U << 0) )
#define CLK_GPIOB_EN() ( RCC->AHB1ENR |= (0x1U << 1) )
#define CLK_GPIOC_EN() ( RCC->AHB1ENR |= (0x1U << 2) )
#define CLK_GPIOD_EN() ( RCC->AHB1ENR |= (0x1U << 3) )
#define CLK_GPIOE_EN() ( RCC->AHB1ENR |= (0x1U << 4) )
#define CLK_GPIOF_EN() ( RCC->AHB1ENR |= (0x1U << 5) )
#define CLK_GPIOG_EN() ( RCC->AHB1ENR |= (0x1U << 6) )
#define CLK_GPIOH_EN() ( RCC->AHB1ENR |= (0x1U << 7) )
#define CLK_GPIOI_EN() ( RCC->AHB1ENR |= (0x1U << 8) )

/* ------------ Enable Clock some Function on AHB bus --------------- */
#define CLK_CRC_EN() 	 					( RCC->AHB1ENR |= (0x1U << 12) )
#define CLK_CCMDATARAM_EN() 	 	( RCC->AHB1ENR |= (0x1U << 20) )
#define CLK_DMA1_EN() 	 				( RCC->AHB1ENR |= (0x1U << 21) )
#define CLK_DMA2_EN() 	 				( RCC->AHB1ENR |= (0x1U << 22) )

/*------------------ Enable Clock Ethernet on AHB bus ---------------*/
#define CLK_ETHMAC_EN() 	 			( RCC->AHB1ENR |= (0x1U << 25) )
#define CLK_ETHMACTXE_EN() 			( RCC->AHB1ENR |= (0x1U << 26) )
#define CLK_ETHMACRXE_EN() 			( RCC->AHB1ENR |= (0x1U << 27) )
#define CLK_ETHMACPTP_EN() 			( RCC->AHB1ENR |= (0x1U << 28) )

#define CLK_OTGHS_EN() 					( RCC->AHB1ENR |= (0x1U << 29) )
#define CLK_OTGHSULPI_EN() 			( RCC->AHB1ENR |= (0x1U << 30) )

/*========================================================================*/
/* Enable Clock some GPIO on AHB bus */
#define CLK_GPIOA_DI() ( RCC->AHB1ENR &= ~(0x1U << 0) )
#define CLK_GPIOB_DI() ( RCC->AHB1ENR &= ~ (0x1U << 1) )
#define CLK_GPIOC_DI() ( RCC->AHB1ENR &= ~ (0x1U << 2) )
#define CLK_GPIOD_DI() ( RCC->AHB1ENR &= ~ (0x1U << 3) )
#define CLK_GPIOE_DI() ( RCC->AHB1ENR &= ~ (0x1U << 4) )
#define CLK_GPIOF_DI() ( RCC->AHB1ENR &= ~ (0x1U << 5) )
#define CLK_GPIOG_DI() ( RCC->AHB1ENR &= ~ (0x1U << 6) )
#define CLK_GPIOH_DI() ( RCC->AHB1ENR &= ~ (0x1U << 7) )
#define CLK_GPIOI_DI() ( RCC->AHB1ENR &= ~ (0x1U << 8) )
/* Disable Clock some Function on AHB bus */
#define CLK_CRC_DI() 	 					( RCC->AHB1ENR &= ~ (0x1U << 12) )
#define CLK_CCMDATARAM_DI() 	 	( RCC->AHB1ENR &= ~ (0x1U << 12) )
#define CLK_DMA1_DI() 	 				( RCC->AHB1ENR &= ~ (0x1U << 12) )
#define CLK_DMA2_DI() 	 				( RCC->AHB1ENR &= ~ (0x1U << 12) )
/* Disable Clock Ethernet on AHB bus */
#define CLK_ETHMAC_DI() 	 			( RCC->AHB1ENR &= ~ (0x1U << 25) )
#define CLK_ETHMACTXE_DI() 			( RCC->AHB1ENR &= ~ (0x1U << 26) )
#define CLK_ETHMACRXE_DI() 			( RCC->AHB1ENR &= ~ (0x1U << 27) )
#define CLK_ETHMACPTP_DI() 			( RCC->AHB1ENR &= ~ (0x1U << 28) )

#define CLK_OTGHS_DI() 					( RCC->AHB1ENR &= ~ (0x1U << 29) )
#define CLK_OTGHSULPI_DI() 			( RCC->AHB1ENR &= ~ (0x1U << 30) )

/* define GPIO Pin */
#define PIN0                                ( 1 << 0 )
#define PIN1                                ( 1 << 1 )
#define PIN2                                ( 1 << 2 )
#define PIN3                                ( 1 << 3 )
#define PIN4                                ( 1 << 4 )
#define PIN5                                ( 1 << 5 )
#define PIN6                                ( 1 << 6 )
#define PIN7                                ( 1 << 7 )
#define PIN8                                ( 1 << 8 )
#define PIN9                                ( 1 << 9 )
#define PIN10                               ( 1 << 10 )
#define PIN11                               ( 1 << 11 )
#define PIN12                               ( 1 << 12 )
#define PIN13                               ( 1 << 13 )
#define PIN14                               ( 1 << 14 )
#define PIN15                               ( 1 << 15 )
#define PIN_All															((uint16_t)0xFFFF)

/* define bit num for LED pin*/
#define LED(PinShift2Bit)										(PinShift2Bit << 2)

/** @define Bit banding 
	@syntax: bit_word_addr = bit_band_base + (byte_offset x 32) + (bit_number × 4)
*/

#define BITBAND(addr, bitnum) ((0x40000000 + 0x2000000)+((addr &0xFFFFF) << 5)+(bitnum << 2))  /* bit_band_address = alias_region_base + (region_base_offset x 32) + (bit_number x 4) */
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

/* Declare Function Write Digital Pin for Ports on STM32F4x */
#define Digital_PortA(Pin, Command)			( BIT_ADDR((GPIOA_BASE + 0x14UL ), Pin) = Command )
#define Digital_PortB(Pin, Command)			( BIT_ADDR((GPIOB_BASE + 0x14UL ), Pin) = Command )
#define Digital_PortC(Pin, Command)			( BIT_ADDR((GPIOC_BASE + 0x14UL ), Pin) = Command )
#define Digital_PortD(Pin, Command)			( BIT_ADDR((GPIOD_BASE + 0x14UL ), Pin) = Command )
#define Digital_PortE(Pin, Command)			( BIT_ADDR((GPIOE_BASE + 0x14UL ), Pin) = Command )
#define Digital_PortF(Pin, Command)			( BIT_ADDR((GPIOF_BASE + 0x14UL ), Pin) = Command )
#define Digital_PortG(Pin, Command)			( BIT_ADDR((GPIOG_BASE + 0x14UL ), Pin) = Command )
#define Digital_PortH(Pin, Command)			( BIT_ADDR((GPIOH_BASE + 0x14UL ), Pin) = Command )
#define Digital_PortI(Pin, Command)			( BIT_ADDR((GPIOI_BASE + 0x14UL ), Pin) = Command )

/* Declare Function read Digital Pin for Ports on STM32F4x */
#define ReadD_PA(Pin)					( BIT_ADDR((GPIOA_BASE + 0x10UL ), Pin) )
#define ReadD_PB(Pin)					( BIT_ADDR((GPIOB_BASE + 0x10UL ), Pin) )
#define ReadD_PC(Pin)					( BIT_ADDR((GPIOC_BASE + 0x10UL ), Pin) )
#define ReadD_PD(Pin)					( BIT_ADDR((GPIOD_BASE + 0x10UL ), Pin) )
#define ReadD_PE(Pin)					( BIT_ADDR((GPIOE_BASE + 0x10UL ), Pin) )
#define ReadD_PF(Pin)					( BIT_ADDR((GPIOF_BASE + 0x10UL ), Pin) )
#define ReadD_PG(Pin)					( BIT_ADDR((GPIOG_BASE + 0x10UL ), Pin) )
#define ReadD_PH(Pin)					( BIT_ADDR((GPIOH_BASE + 0x10UL ), Pin) )
#define ReadD_PI(Pin)					( BIT_ADDR((GPIOI_BASE + 0x10UL ), Pin) )

/** 
  * @brief  Status structures definition  
  */  
typedef enum 
{
  OK       = 0x00U,
  ERROR    = 0x01U,
  BUSY     = 0x02U,
  TIMEOUT  = 0x03U
} StatusTypeDef;

typedef struct
{
  uint32_t SYSCLK_Freq; /*!<  SYSCLK clock frequency expressed in Hz */
  uint32_t HCLK_Freq;   /*!<  HCLK clock frequency expressed in Hz   */
  uint32_t PCLK1_Freq;  /*!<  PCLK1 clock frequency expressed in Hz  */
  uint32_t PCLK2_Freq;  /*!<  PCLK2 clock frequency expressed in Hz  */
}Clock_Typedef;

#if !defined  (HSEINPUT_VALUE) 
  #define HSEINPUT_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
  
#endif /* HSE_VALUE */
#if !defined  (HSI_RC)   
  #define HSI_RC    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */  

#define MAX_DELAY      0xFFFFFFFFU

#define HAL_IS_BIT_SET(REG, BIT)         (((REG) & (BIT)) == (BIT))
#define HAL_IS_BIT_CLR(REG, BIT)         (((REG) & (BIT)) == 0U)

#define __HAL_LINKDMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__)               \
                        do{                                                      \
                              (__HANDLE__)->__PPP_DMA_FIELD__ = &(__DMA_HANDLE__); \
                              (__DMA_HANDLE__).Parent = (__HANDLE__);             \
                          } while(0U)
								
													
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /* ARM Compiler V6 */
  #ifndef __weak
    #define __weak  __attribute__((weak))
  #endif
  #ifndef __packed
    #define __packed  __attribute__((packed))
  #endif
#elif defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
  #ifndef __weak
    #define __weak   __attribute__((weak))
  #endif /* __weak */
  #ifndef __packed
    #define __packed __attribute__((__packed__))
  #endif /* __packed */
#endif /* __GNUC__ */


/* Macro to get variable aligned on 4-bytes, for __ICCARM__ the directive "#pragma data_alignment=4" must be used instead */
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /* ARM Compiler V6 */
  #ifndef __ALIGN_BEGIN
    #define __ALIGN_BEGIN
  #endif
  #ifndef __ALIGN_END
    #define __ALIGN_END      __attribute__ ((aligned (4)))
  #endif
#elif defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
  #ifndef __ALIGN_END
#define __ALIGN_END    __attribute__ ((aligned (4)))
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN  
    #define __ALIGN_BEGIN
  #endif /* __ALIGN_BEGIN */
#else
  #ifndef __ALIGN_END
    #define __ALIGN_END
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN      
    #if defined   (__CC_ARM)      /* ARM Compiler V5*/
#define __ALIGN_BEGIN    __align(4)
    #elif defined (__ICCARM__)    /* IAR Compiler */
      #define __ALIGN_BEGIN 
    #endif /* __CC_ARM */
  #endif /* __ALIGN_BEGIN */
#endif /* __GNUC__ */


/** 
  * @brief  __RAM_FUNC definition
  */ 
#if defined ( __CC_ARM   ) || (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) || defined   (  __GNUC__  )
/* ARM V4/V5 and V6 & GNU Compiler
   -------------------------------
*/
#define __NOINLINE __attribute__ ( (noinline) )

#elif defined ( __ICCARM__ )
/* ICCARM Compiler
   ---------------
*/
#define __NOINLINE _Pragma("optimize = no_inline")

#endif

#ifdef __cplusplus
}
#endif


#endif /* ___STM32F4xx_HAL_DEF */
