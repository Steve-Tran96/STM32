/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
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

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdio.h>
#include <string.h> 

#include "rcc.h"
#include "Sys.h"
#include "gpio.h"
#include "exti.h"
#include "usart.h"
#include "stm32f4xx_it.h"
#include "dma.h"


void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
