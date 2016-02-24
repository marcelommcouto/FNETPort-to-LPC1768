#include "fnet_config.h"

#include "fnet.h"
#include "stack/fnet_timer_prv.h"
#include "stack/fnet_isr.h"

#ifdef FNET_LPC

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lpc17xx_timer.h"

void fnet_cpu_timer_handler_top(void *cookie);
void fnet_cpu_timer_handler_bottom(void *cookie);

fnet_return_t fnet_cpu_timer_init( fnet_time_t period_ms )
{
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_MATCHCFG_Type TIM_MatchConfigStruct;

	/* Initialize timer 0, prescale count time of 1000uS = 1mS */
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue  = 1000;

	TIM_MatchConfigStruct.MatchChannel = 0;							/* Use channel 0, MR0. */
	TIM_MatchConfigStruct.IntOnMatch = TRUE;						/* Enable interrupt when MR0 matches the value in TC register. */
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;						/* Enable reset on MR0: TIMER will reset if MR0 matches it. */
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;						/* Stop on MR0 if MR0 matches it. */
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;	/* Toggle MR0.0 pin if MR0 matches it. */
	TIM_MatchConfigStruct.MatchValue = period_ms;					/* Set Match value, count value of 10 (10 * 10ms = 100ms). */

	TIM_Init(LPC_TIM2, TIM_TIMER_MODE, &TIM_ConfigStruct);			/* Set configuration for Tim_config and Tim_MatchConfig. */
	TIM_ConfigMatch(LPC_TIM2,&TIM_MatchConfigStruct);

	NVIC_SetPriority(TIMER2_IRQn, FNET_CFG_CPU_TIMER_VECTOR_PRIORITY);
	NVIC_EnableIRQ(TIMER2_IRQn);														/* Enable interrupt for timer 0 */

	fnet_isr_vector_init(TIMER2_IRQn,fnet_cpu_timer_handler_top,fnet_timer_handler_bottom,
	                FNET_CFG_CPU_TIMER_VECTOR_PRIORITY,0);

	TIM_Cmd(LPC_TIM2,ENABLE);															/* To start timer 0. */

	return FNET_OK;
}

/************************************************************************
* NAME: fnet_cpu_timer_release
*
* DESCRIPTION: Relaeses TCP/IP hardware timer.
*
*************************************************************************/
void fnet_cpu_timer_release( void )
{
	NVIC_DisableIRQ(TIMER2_IRQn);		/* Disable interrupt for timer. */
	TIM_Cmd(LPC_TIM2, DISABLE);			/* Disable timer. */
}
//
void fnet_cpu_timer_handler_top(void *cookie)
{
	TIM_ClearIntPending(LPC_TIM2, 0);
	fnet_timer_ticks_inc();
}
//
void fnet_cpu_timer_handler_bottom(void *cookie)
{
	fnet_timer_handler_bottom((uint32_t)cookie);
}
//
void TIMER2_IRQHandler(void)
{
	fnet_isr_handler(TIMER2_IRQn);
}

#endif
