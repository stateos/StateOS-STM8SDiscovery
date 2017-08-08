/******************************************************************************

    @file    StateOS: osport.h
    @author  Rajmund Szymanski
    @date    08.08.2017
    @brief   StateOS port definitions for STM8S uC.

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

#ifndef __STATEOSPORT_H
#define __STATEOSPORT_H

#include <stm8s.h>
#include <osconfig.h>
#include <osdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15);
INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler,     16);

/* -------------------------------------------------------------------------- */

#ifndef  OS_TICKLESS
#define  OS_TICKLESS          0 /* os not works in tick-less mode             */
#endif

#if      OS_TICKLESS
#error   osconfig.h: Incorrect OS_TICKLESS value! This port does not support tick-less mode.
#endif

/* -------------------------------------------------------------------------- */

#ifndef CPU_FREQUENCY
#error   osconfig.h: Undefined CPU_FREQUENCY value!
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_FREQUENCY
#define  OS_FREQUENCY      1000 /* Hz */
#endif

#if     (OS_FREQUENCY > 1000)
#error   osconfig.h: Incorrect OS_FREQUENCY value!
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_ROBIN
#define  OS_ROBIN             0 /* system works in cooperative mode           */
#endif

#if     (OS_ROBIN > OS_FREQUENCY)
#error   osconfig.h: Incorrect OS_ROBIN value!
#endif

/* -------------------------------------------------------------------------- */

// force yield system control to the next process
__STATIC_INLINE
void port_ctx_switch( void )
{
	TIM3->EGR = TIM3_EGR_CC1G;
}

/* -------------------------------------------------------------------------- */

// reset context switch indicator
__STATIC_INLINE
void port_ctx_reset( void )
{
}

/* -------------------------------------------------------------------------- */

// clear time breakpoint
__STATIC_INLINE
void port_tmr_stop( void )
{
}
	
/* -------------------------------------------------------------------------- */

// set time breakpoint
__STATIC_INLINE
void port_tmr_start( uint32_t timeout )
{
	(void) timeout;
}

/* -------------------------------------------------------------------------- */

// force timer interrupt
__STATIC_INLINE
void port_tmr_force( void )
{
}

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------- */

#endif//__STATEOSPORT_H
