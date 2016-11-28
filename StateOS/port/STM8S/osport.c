/******************************************************************************

    @file    StateOS: osport.c
    @author  Rajmund Szymanski
    @date    28.10.2016
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
	CLK->SWCR  |= CLK_SWCR_SWEN; // Enable switching the master clock to the source defined in the CLK_SWR register
	CLK->SWR    = 0xB4;          // HSE selected as master clock source

	TIM4->PSCR  = 6;             // Prescaler:      16.000.000 /  64 = 250.000
	TIM4->ARR   = 249;           // Auto-reload value: 250.000 / 250 =   1.000
	TIM4->IER  |= TIM4_IER_UIE;  // Enable interrupt
	TIM4->CR1  |= TIM4_CR1_CEN;  // Enable timer

	enableInterrupts();

/******************************************************************************
 End of configuration
*******************************************************************************/
}

/* -------------------------------------------------------------------------- */

/******************************************************************************
 Put here the procedure of interrupt handler of system timer for non-tick-less mode
*******************************************************************************/

@interrupt//@svlreg
void TIM4_UPD_OVF_IRQHandler( void )
{
	TIM4->SR1 = ~TIM4_SR1_UIF; // clear timer's status register

	System.cnt++;
#if OS_ROBIN
	core_tmr_handler();
	System.dly++;
	if (System.dly >= OS_FREQUENCY/OS_ROBIN)
	port_ctx_switch();
#endif
}

/******************************************************************************
 End of the procedure of interrupt handler
*******************************************************************************/

/* -------------------------------------------------------------------------- */
