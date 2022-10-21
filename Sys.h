/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Sys.h
  * @brief          : Header for Sys.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 Steve Tran.
  * All rights reserved.
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __SYS_H
#define __SYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f407xx.h"
#include "stm32f4xx_def.h"

#define System_Tick_Load  1000000UL

/** @defgroup TICK_FREQ Tick Frequency */
typedef enum
{
  TICK_FREQ_10HZ         = 100U,
  TICK_FREQ_100HZ        = 10U,
  TICK_FREQ_1KHZ         = 1U,
  TICK_FREQ_DEFAULT      = TICK_FREQ_1KHZ
} I_TickFreqTypeDef;

extern __IO uint32_t u32Tick;
extern uint32_t u32TickPrio;
extern I_TickFreqTypeDef enuTickFreq;

StatusTypeDef Init_SysTick(void);
StatusTypeDef InitTick(uint32_t TickPriority, uint32_t SysLoad);
void IncTick(void);
uint32_t GetTick(void);
void Delay_ms(uint32_t ms);
void Delay_ns(uint32_t ns);
void SysTick_Handler(void);
__STATIC_INLINE uint32_t SystemCLK_Config(uint32_t tick);
void SystemClock_Config(void);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_H */

