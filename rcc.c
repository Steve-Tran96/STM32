#include "rcc.h"

/* Private variables ---------------------------------------------------------*/
static __I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

/* Declare variable save System Clock*/
Clock_Typedef Get_Clock;

/**
  * @brief  Initializes the RCC Oscillators according to the specified parameters in the
*/
StatusTypeDef SystemClock_Config_HSE(void){
	/* Disable PLL */
	PLL_DISABLE();
	
	/* Set HSE On */
	RCC->CR |= (0x1UL << 16);
	
	/* Wait till HSE is ready */
	while((RCC->CR & (0x1U<<17)) == 0){
		// if timeout is greater than 100U, return TIMEOUT;
	}
	
	/* Check Flag HSE READY */
	if((RCC->CR & (0x1U<<17)) == 1){
		/* Set PWR on: Power interface clock enable */
		RCC->APB1ENR |= (0x1UL << 28);
    PWR->CR |= PWR_CR_VOS;	
	}

	/* Configuring M, N, P, Q, AHB, APB */
		/* Set HPRE: AHB Prescaler - /1 */
		RCC->CFGR |= (0x0U << 4);
	
		/* Set PPRE2: APB2 High Speed - /2*/
		RCC->CFGR |= (0x4U << 13);
		
		/* Set PPRE1: APB1 Low Speed /4*/
		RCC->CFGR |= (0x5U << 10);
		
		/* HSE oscillator clock selected as PLL */
		RCC->PLLCFGR |= (0x1U << 22U);
		
		/** @PLLM division with 2 = PLLM = 63
			@div: 8
		*/
		RCC->PLLCFGR |= (8 << 1U);
		
		/**@PLLN: Main PLL multiplication with 50 = PLLN = 432
			@mul: 336
		*/
		RCC->PLLCFGR |=	(336 << 6U);
		
		/** @PLLP: Main PLLP with PLLP = 2, 4, 6, or 8 
			@div: 2
		*/
		RCC->PLLCFGR |= (0x0U << 16);
		
		/** @PLLQ: Main PLLP with 2 = PLLQ = 15
		@div: 4
		*/
		RCC->PLLCFGR |= (0x4UL << 24);
		
		/* Set PLL On */
		RCC->CR |= (0x1UL << 24);
		
		/* Wait till PLL is ready */
		while((RCC->CR & (0x1U<<25)) == 0){
			/*if timeout is greater than 2U, return TIMEOUT;*/
		}
		
		/* Configure Flash prefetch, Instruction cache, Data cache and wait state */
		FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
		
			
		/* PLL select as System Clock */ 
		RCC->CFGR |= (0x1U << 2U);
				
		/* Set Select System Clock */
		RCC->CFGR |= (0x2U << 0);
		
		/* Wait till SWS = 1 */
		while((RCC->CFGR & (0x2U << 2)) == 0){
			/*if timeout is greater than 2U, return TIMEOUT;*/
		}
	
		/*Get System Clock (SYSCLK, HCLK, HSI, HSE) */
	RCC_GetClocksFreq(&Get_Clock);
		
		/*Update new SystemCoreClock varial */
	SystemCoreClock = Get_Clock.SYSCLK_Freq;
		
	return OK;
}

void RCC_GetClocksFreq(Clock_Typedef* FreqClock){
  uint32_t tmp = 0, presc = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;

  /** @Get System Clock via 2 bits of SWS CFGR[2:3] 
	@00: HSI
	@01: HSE
	@10: PLL
	@11: Not
	*/

  tmp = RCC->CFGR & RCC_CFGR_SWS; 
  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock source */
      FreqClock->SYSCLK_Freq = HSI_RC;
      break;
    case 0x04:  /* HSE used as system clock  source */
      FreqClock->SYSCLK_Freq  = HSEINPUT_VALUE;
      break;
    case 0x08:  /* PLL used as system clock  source */

      /** @PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
         @SYSCLK = PLL_VCO / PLLP
       */    
      pllsource = (RCC->PLLCFGR & (0x1UL << 22)) >> 22; /*bit 22 in CFGR register - PLLSRC*/
		
		/* Get PLLM div */
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
      
		/* Bit PLLSRC is 1 -> PLL as Clock Source */
      if (pllsource != 0)
      {
        /* HSE used as PLL clock source */
        pllvco = (HSEINPUT_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6); /* Get PLLM div */
      }
      else
      {
        /* HSI used as PLL clock source */
        pllvco = (HSI_RC / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);      
      }
			
			/* Get PLLP multi */
      pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> 16) + 1 ) *2;  /* 00,01,10,11 <=> 2,4,6,8 VD: /2 => 00 >> (16 + 1)*2 = 1*2 = 2*/
			
			/* Clock Frequency is divison between pllvco and pllp  */
      FreqClock->SYSCLK_Freq  = pllvco/pllp;
      break;
			
    default:
      FreqClock->SYSCLK_Freq  = HSI_RC;
      break;
  }
  /* Compute HCLK, PCLK1 and PCLK2 clocks frequencies ------------------------*/

  /* Get HCLK prescaler via bit HPRE of CFGR register*/
  tmp = RCC->CFGR & RCC_CFGR_HPRE; 
  tmp = tmp >> 4;	/*VD /2 -> 1000 0000 >> 4 = 1000*/
  presc = APBAHBPrescTable[tmp]; /*VD 1000 <=> 8 => APBAHBPrescTable[8] = 1 */
	
  /* HCLK clock frequency */
  FreqClock->HCLK_Freq = FreqClock->SYSCLK_Freq >> presc;

  /* Get PCLK1 prescaler */
  tmp = RCC->CFGR & RCC_CFGR_PPRE1;
  tmp = tmp >> 10;
  presc = APBAHBPrescTable[tmp];
  /* PCLK1 clock frequency */
  FreqClock->PCLK1_Freq = FreqClock->SYSCLK_Freq >> presc;

  /* Get PCLK2 prescaler */
  tmp = RCC->CFGR & RCC_CFGR_PPRE2;
  tmp = tmp >> 13;
  presc = APBAHBPrescTable[tmp];
  /* PCLK2 clock frequency */
  FreqClock->PCLK2_Freq = FreqClock->SYSCLK_Freq >> presc;
}




