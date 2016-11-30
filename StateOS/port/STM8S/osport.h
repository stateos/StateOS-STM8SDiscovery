/******************************************************************************

    @file    StateOS: osport.h
    @author  Rajmund Szymanski
    @date    30.11.2016
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

#include <osconfig.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_TIMER
#define  OS_TIMER             0
#endif

#if      OS_TIMER > 0
#error   osconfig.h: Incorrect OS_TIMER value! This port doesn't support tick-less mode.
#endif

/* -------------------------------------------------------------------------- */

#define  Counter System.cnt
#define  Current System.cur

/* -------------------------------------------------------------------------- */

#ifndef CPU_FREQUENCY
#error   osconfig.h: Undefined CPU_FREQUENCY value!
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_FREQUENCY
#define  OS_FREQUENCY      1000 /* Hz */
#else

#if     (OS_FREQUENCY > 1000)
#error   osconfig.h: Incorrect OS_FREQUENCY value!
#endif

#endif //OS_FREQUENCY

/* -------------------------------------------------------------------------- */

#ifndef  OS_ROBIN
#define  OS_ROBIN             0 /* system works in cooperative mode           */
#endif

#if     (OS_ROBIN > OS_FREQUENCY)
#error   osconfig.h: Incorrect OS_ROBIN value!
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_HEAP_SIZE
#define  OS_HEAP_SIZE         0 /* default system heap: all free memory       */
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_STACK_SIZE
#define  OS_STACK_SIZE      256 /* default task stack size in bytes           */
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_LOCK_LEVEL
#define  OS_LOCK_LEVEL        0 /* critical section blocks all interrupts */
#endif

#if      OS_LOCK_LEVEL > 0
#error   osconfig.h: Incorrect OS_LOCK_LEVEL value! Must be 0.
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_MAIN_PRIO
#define  OS_MAIN_PRIO         0 /* priority of main process */
#endif

/* -------------------------------------------------------------------------- */

typedef  uint8_t              stk_t;

/* -------------------------------------------------------------------------- */

extern   stk_t               _stack[];
#define  MAIN_SP             _stack+1

/* -------------------------------------------------------------------------- */

static inline
void port_ctx_reset( void )
{
	TIM3->SR1 = ~TIM3_SR1_CC1IF;
}

/* -------------------------------------------------------------------------- */

static inline
void port_ctx_switch( void )
{
	TIM3->EGR = TIM3_EGR_CC1G;
}

/* -------------------------------------------------------------------------- */

// clear time breakpoint
static inline
void port_tmr_stop( void )
{
}
	
/* -------------------------------------------------------------------------- */

// set time breakpoint
static inline
void port_tmr_start( unsigned timeout )
{
	(void) timeout;
}

/* -------------------------------------------------------------------------- */

// force timer interrupt
static inline
void port_tmr_force( void )
{
}

/* -------------------------------------------------------------------------- */

#ifndef  __ALWAYS
#define  __ALWAYS
#endif
#ifndef  __CONSTRUCTOR
#define  __CONSTRUCTOR
#endif
#ifndef  __NORETURN
#define  __NORETURN
#endif
#ifndef  __WEAK
#define  __WEAK
#endif
#ifndef  __WFI
#define  __WFI  wfi
#endif

/* -------------------------------------------------------------------------- */

#define  port_get_sp()     (void *)_asm("ldw x, sp")
#define  port_set_sp(sp)           _asm("ldw sp, x", (unsigned)(sp))

#define  port_get_lock()     (char)_asm("push cc""\n""pop a")
#define  port_put_lock(state)      _asm("push a""\n""pop cc", (char)(state))


#define  port_set_lock()            disableInterrupts()
#define  port_clr_lock()            enableInterrupts()

#define  port_sys_lock()       do { char __LOCK = port_get_lock(); port_set_lock()
#define  port_sys_unlock()          port_put_lock(__LOCK); } while(0)

#define  port_sys_enable()     do { (void)0
#define  port_sys_disable()         (void)0; } while(0)

#define  port_isr_lock()       do { port_set_lock()
#define  port_isr_unlock()          port_clr_lock(); } while(0)

#define  port_isr_enable()     do { (void)0
#define  port_isr_disable()         (void)0; } while(0)

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------- */

#endif//__STATEOSPORT_H
