#include "Sys.h"
#include <stdint.h>

/* When use CMSIS not include via KeilC -> define STM32Fxx to use CMSIS library */

/** @addtogroup Private_Variables 
*/
__IO uint32_t u32Tick;
uint32_t u32TickPrio   = (1UL << __NVIC_PRIO_BITS); /* Invalid PRIO */
I_TickFreqTypeDef enuTickFreq = TICK_FREQ_DEFAULT;  /* 1KHz */ 


/* ----- Explain NVIC_SetPriorityGrouping(PriorityGroupTmp) function --------------
  uint32_t reg_value;
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);           

  reg_value  =  SCB->AIRCR;                                                   
  reg_value &= ~((uint32_t)(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk)); 
  reg_value  =  (reg_value                                   |
                ((uint32_t)0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                (PriorityGroupTmp << SCB_AIRCR_PRIGROUP_Pos)  );            
  SCB->AIRCR =  reg_value;
*/

StatusTypeDef Init_SysTick(void){
	/* Set Interrupt Group Priority */
	/*!< 4 bits for pre-emption priority - 0 bits for subpriority: GroupPriorities[16], SubPriorities[0] */
	NVIC_SetPriorityGrouping(0x00000003U);
	/*!< 0x15U: tick interrupt priority [0..15] */
	InitTick(15U, System_Tick_Load);
	
	return OK;
}

	/**@
	
	*/
StatusTypeDef InitTick(uint32_t TickPriority, uint32_t SysLoad){
	uint32_t prioritygroup = 0x00U;
	
	/**	@T=1/f = 1/168000000 s
			@=> 1/1000000 s = 168
	*/
	if(SystemCLK_Config(SystemCoreClock / (SysLoad / enuTickFreq)) > 0U){
		return ERROR;
	}
	 /* Configure the SysTick IRQ priority */
	if(TickPriority <= __NVIC_PRIO_BITS){
		prioritygroup = NVIC_GetPriorityGrouping();
		
		/* NVIC_SetPriority(IRQ , ... )*/
		/* NVIC_EncodePriority(IRQ ,PreemptPriority, SubPriority)*/
		NVIC_SetPriority(SysTick_IRQn , NVIC_EncodePriority(prioritygroup, TickPriority, 0U));

		u32TickPrio = TickPriority;			
	}
	else
	{
			return ERROR;
	}
	
	return OK;
}


/**
  * @brief Provides a tick value in millisecond.
  * @note This function is declared as __weak to be overwritten in case of other 
  *       implementations in user file.
  * @retval tick value
  */
__weak uint32_t GetTick(void)
{
  return u32Tick;
}

/**
  * @brief This function provides minimum delay (in milliseconds) based 
  *        on variable incremented.
  * @note In the default implementation , SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals where uwTick
  *       is incremented.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @param Delay specifies the delay time length, in milliseconds.
  * @retval None
  */
__weak void Delay_ms(uint32_t ms)
{
  uint32_t tickstart = GetTick();
  uint32_t wait = ms*1000;

  /* Add a freq to guarantee minimum wait */
  if (wait < MAX_DELAY)
  {
    wait += (uint32_t)(enuTickFreq);
  }
	
  while((GetTick() - tickstart) < wait)
  {
		
  }
}

/* Enable after InitTick = System_Clock/ */
__weak void Delay_ns(uint32_t ns)
{
  uint32_t tickstart = GetTick();
  uint32_t wait = ns;

  /* Add a freq to guarantee minimum wait */
  if (wait < MAX_DELAY)
  {
    wait += (uint32_t)(enuTickFreq);
  }
	
  while((GetTick() - tickstart) < wait)
  {
		
  }
}

/**
  * @brief This function is called to increment  a global variable "u32Tick"
  *        used as application time base.
  * @note In the default implementation, this variable is incremented each 1ms
  *       in SysTick ISR.
 * @note This function is declared as __weak to be overwritten in case of other 
  *      implementations in user file.
  * @retval None
  */

void SysTick_Handler(void)
{
	/* u32Tick ++ when SysTick Interrupt */
  u32Tick += enuTickFreq;
	
}

__STATIC_INLINE uint32_t SystemCLK_Config(uint32_t ticks){
	
	/* Checking ticks > 0xff ffff (24bit) */
	if(ticks > 0xFFFFFF){
		return (1UL);
	}
	/**@SysTick interrupt is required every "ticks" - 1 */
	SysTick->LOAD = (uint32_t)(ticks - 1UL); /* Start couted from Zero */
	/* The current value of the SysTick counter. */
	SysTick->VAL = 0UL;
	/* Configruate and Enable Systick */
	SysTick->CTRL |= ((0x1UL << 0) | (0x1UL << 1) | (0x1UL << 2)); /* ENABLE , TICKINT counting down -> exception request, CLOCKSOURCE /1 */
	
	return (0UL);
}


void SystemClock_Config(void){
	
	
}
