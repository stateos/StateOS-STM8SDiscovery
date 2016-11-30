/******************************************************************************

    @file    StateOS: osport.c
    @author  Rajmund Szymanski
    @date    30.11.2016
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
/******************************************************************************
 Put here configuration of system timer for non-tick-less mode
*******************************************************************************/

	CLK->CKDIVR = 0;
	CLK->ECKR  |= CLK_ECKR_HSEEN; while ((CLK->ECKR & CLK_ECKR_HSERDY) == 0);
	CLK->SWCR  |= CLK_SWCR_SWEN;
	CLK->SWR    = 0xB4; /* HSE */ while ((CLK->SWCR & CLK_SWCR_SWBSY)  == 1);

#define LEN_(X)  ((X)?LEN_((X)>>1)+1:0)
#define PSC_ LEN_((CPU_FREQUENCY/OS_FREQUENCY-1)>>16)
#define ARR_      (CPU_FREQUENCY/OS_FREQUENCY/(1<<PSC_))

	TIM3->PSCR  = (PSC_);
	TIM3->ARRH  = (ARR_ - 1) >> 8;
	TIM3->ARRL  = (ARR_ - 1);
	TIM3->IER  |= TIM3_IER_UIE;
	TIM3->IER  |= TIM3_IER_CC1IE;
	TIM3->CR1  |= TIM3_CR1_CEN;

	enableInterrupts();

/******************************************************************************
 End of configuration
*******************************************************************************/
}

/* -------------------------------------------------------------------------- */

/******************************************************************************
 Put here the procedure of interrupt handler of system timer for non-tick-less mode
*******************************************************************************/

#if OS_ROBIN
@svlreg
#endif
@interrupt
void TIM3_UPD_OVF_BRK_IRQHandler( void )
{
	TIM3->SR1= ~TIM3_SR1_UIF;

	System.cnt++;
#if OS_ROBIN
	core_tmr_handler();
	System.dly++;
	if (System.dly < OS_FREQUENCY/OS_ROBIN) return;
	port_set_sp(core_tsk_handler(port_get_sp()));
#endif
}

@svlreg
@interrupt
void TIM3_CAP_COM_IRQHandler( void )
{
	TIM3->SR1 = ~TIM3_SR1_CC1IF;

	port_set_sp(core_tsk_handler(port_get_sp()));
}

/******************************************************************************
 End of the procedure of interrupt handler
*******************************************************************************/

/* -------------------------------------------------------------------------- */
