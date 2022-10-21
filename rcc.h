/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Sys.h
  * @brief          : Header for rcc.c file.
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

#ifndef __RCC_H
#define __RCC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_def.h"

#define PLL_ENABLE() (*(volatile uint32_t *) (0x42000000UL + (0x00023800UL * 32U) + (0x18U * 4U)) = 1)
#define PLL_DISABLE() (*(volatile uint32_t *) (0x42000000UL + (0x00023800UL * 32U) + (0x18U * 4U)) = 0)
#define NULL ((void *)0)

extern Clock_Typedef Get_Clock;

StatusTypeDef SystemClock_Config_HSE(void);
void RCC_GetClocksFreq(Clock_Typedef *FreqClock);


#ifdef __cplusplus
}
#endif

#endif /* __RCC_H */

