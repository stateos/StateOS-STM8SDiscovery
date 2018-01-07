/******************************************************************************

    @file    StateOS: osport.c
    @author  Rajmund Szymanski
    @date    05.01.2018
    @brief   StateOS port file for STM8S uC.

 ******************************************************************************

    StateOS - Copyright (C) 2013 Rajmund Szymanski.

    This file is part of StateOS distribution.

    StateOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation; either version 3 of the License,
    or (at your option) any later version.

    StateOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include <oskernel.h>

/* -------------------------------------------------------------------------- */

void port_sys_init( void )
{
	CLK->CKDIVR = 0;
	CLK->ECKR  |= CLK_ECKR_HSEEN; while ((CLK->ECKR & CLK_ECKR_HSERDY) == 0);
	CLK->SWCR  |= CLK_SWCR_SWEN;
	CLK->SWR    = 0xB4; /* HSE */ while ((CLK->SWCR & CLK_SWCR_SWBSY)  == 1);

#if HW_TIMER_SIZE == 0

	#define  CNT_(X)   ((X)>>0?(X)>>1?(X)>>2?(X)>>3?(X)>>4?(X)>>5?(X)>>6?(X)>>7?(X)>>8?(X)>>9?1/0:9:8:7:6:5:4:3:2:1:0)
	#define  PSC_ CNT_ (((CPU_FREQUENCY)/(OS_FREQUENCY)-1)>>16)
	#define  ARR_     ((((CPU_FREQUENCY)/(OS_FREQUENCY))>>PSC_)-1)

/******************************************************************************
 Non-tick-less mode: configuration of system timer
 It must generate interrupts with frequency OS_FREQUENCY
*******************************************************************************/

	TIM3->PSCR  = PSC_;
	TIM3->ARRH  = (uint8_t)(ARR_ >> 8);
	TIM3->ARRL  = (uint8_t)(ARR_);
	TIM3->CCR1H = 0xFF;
	TIM3->CCR1L = 0xFF;
	TIM3->IER   = TIM3_IER_CC1IE | TIM3_IER_UIE;
	TIM3->CR1   = TIM3_CR1_CEN;

/******************************************************************************
 End of configuration
*******************************************************************************/

#else //HW_TIMER_SIZE

	#if (OS_FREQUENCY)/(OS_ROBIN)-1 > UINT16_MAX
	#error Incorrect OS_ROBIN frequency!
	#endif

	#define  CNT_(X)   ((X)>>0?(X)>>1?(X)>>2?(X)>>3?(X)>>4?(X)>>5?(X)>>6?(X)>>7?(X)>>8?(X)>>9?1/0:9:8:7:6:5:4:3:2:1:0)
	#define  PSC_ CNT_  ((CPU_FREQUENCY)/(OS_FREQUENCY)-1)
	#define  CCR_       ((OS_FREQUENCY)/(OS_ROBIN))

/******************************************************************************
 Tick-less mode: configuration of system timer
 It must be rescaled to frequency OS_FREQUENCY
*******************************************************************************/

	TIM3->PSCR  = PSC_;
	TIM3->CCR1H = (uint8_t)(CCR_ >> 8);
	TIM3->CCR1L = (uint8_t)(CCR_);
	#if HW_TIMER_SIZE < OS_TIMER_SIZE
	TIM3->IER   = TIM3_IER_CC1IE | TIM3_IER_UIE;
	#else
	TIM3->IER   = TIM3_IER_CC1IE;
	#endif
	TIM3->CR1   = TIM3_CR1_CEN;

/******************************************************************************
 End of configuration
*******************************************************************************/

#endif//HW_TIMER_SIZE

	port_clr_lock();
}

/* -------------------------------------------------------------------------- */

#if HW_TIMER_SIZE == 0

/******************************************************************************
 Non-tick-less mode: interrupt handler of system timer
*******************************************************************************/

#if defined(__CSMC__)
@svlreg
#endif
INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
{
//	if (TIM3->SR1 & TIM3_SR1_UIF)
	{
		TIM3->SR1 = (uint8_t) ~TIM3_SR1_UIF;
		core_sys_tick();
	}
}

/******************************************************************************
 End of the handler
*******************************************************************************/

/******************************************************************************
 Interrupt handler for context switch
*******************************************************************************/

#if defined(__CSMC__)
@svlreg
#endif
INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
{
//	if (TIM3->SR1 & TIM3_SR1_CC1IF)
	{
		TIM3->SR1 = (uint8_t) ~TIM3_SR1_CC1IF;
		_set_SP(core_tsk_handler(_get_SP()));
	}
}

/******************************************************************************
 End of the handler
*******************************************************************************/

#else //HW_TIMER_SIZE

/******************************************************************************
 Tick-less mode: interrupt handler of system timer
*******************************************************************************/

#if HW_TIMER_SIZE < OS_TIMER_SIZE

#if defined(__CSMC__)
@svlreg
#endif
INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
{
//	if (TIM3->SR1 & TIM3_SR1_UIF)
	{
		TIM3->SR1 = (uint8_t) ~TIM3_SR1_UIF;
		core_sys_tick();
	}
}

#endif

/******************************************************************************
 End of the handler
*******************************************************************************/

/******************************************************************************
 Interrupt handler for context switch
*******************************************************************************/

#if defined(__CSMC__)
@svlreg
#endif
INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
{
	if (TIM3->SR1 & TIM3_SR1_CC2IF)
	{
		TIM3->SR1 = (uint8_t) ~TIM3_SR1_CC2IF;
		core_tmr_handler();
	}
	if (TIM3->SR1 & TIM3_SR1_CC1IF)
	{
		TIM3->SR1 = (uint8_t) ~TIM3_SR1_CC1IF;
		_set_SP(core_tsk_handler(_get_SP()));
	}
}

/******************************************************************************
 End of the handler
*******************************************************************************/

/******************************************************************************
 Tick-less mode: return current system time
*******************************************************************************/

#if HW_TIMER_SIZE < OS_TIMER_SIZE

cnt_t port_sys_time( void )
{
	cnt_t    cnt;
	uint16_t tck;

	cnt = System.cnt;
	tck = ((uint16_t)TIM3->CNTRH << 8) | TIM3->CNTRL;

	if (TIM3->SR1 & TIM3_SR1_UIF)
	{
		tck = ((uint16_t)TIM3->CNTRH << 8) | TIM3->CNTRL;
		cnt += (cnt_t)(1) << (HW_TIMER_SIZE);
	}

	return cnt + tck;
}

#endif

/******************************************************************************
 End of the function
*******************************************************************************/

#endif//HW_TIMER_SIZE

/* -------------------------------------------------------------------------- */
