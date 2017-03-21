/******************************************************************************

    @file    StateOS: osport.h
    @author  Rajmund Szymanski
    @date    21.03.2017
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

INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15);

/* -------------------------------------------------------------------------- */

#ifndef  OS_TIMER
#define  OS_TIMER             0
#endif

#if      OS_TIMER > 0
#error   osconfig.h: Incorrect OS_TIMER value! This port does not support tick-less mode.
#endif

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
#define  OS_STACK_SIZE      128 /* default task stack size in bytes           */
#endif

#ifndef  OS_IDLE_STACK
#define  OS_IDLE_STACK       32 /* idle task stack size in bytes              */
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

#ifndef  OS_ASSERT
#define  OS_ASSERT            0 /* don't include standard assertions          */
#endif

#if     (OS_ASSERT == 0)
#ifndef  NDEBUG
#define  NDEBUG
#endif
#endif

#include <assert.h>

/* -------------------------------------------------------------------------- */

typedef  uint8_t              stk_t;

/* -------------------------------------------------------------------------- */

#if      defined(__CSMC__)
extern   stk_t               _stack[];
#define  MAIN_TOP            _stack+1
#endif

/* -------------------------------------------------------------------------- */

#if      defined(__CSMC__)

#ifndef  __CONSTRUCTOR
#define  __CONSTRUCTOR
#endif
#ifndef  __NO_RETURN
#define  __NO_RETURN
#endif
#ifndef  __STATIC_INLINE
#define  __STATIC_INLINE      static inline
#endif
#ifndef  __WFI
#define  __WFI                wfi
#endif

#elif    defined(__SDCC)

#ifndef  __CONSTRUCTOR
#define  __CONSTRUCTOR
#endif
#ifndef  __NO_RETURN
#define  __NO_RETURN         _Noreturn
#endif
#ifndef  __STATIC_INLINE
#define  __STATIC_INLINE      static inline
#endif
#ifndef  __WFI
#define  __WFI                wfi
#endif

#endif

/* -------------------------------------------------------------------------- */

#if      defined(__CSMC__)

#define  port_get_lock()     (char)_asm("push cc""\n""pop a")
#define  port_put_lock(state)      _asm("push a""\n""pop cc", (char)(state))

#elif    defined(__SDCC)

char     port_get_lock(void);
void     port_put_lock(char state);

#endif

#define  port_set_lock()            disableInterrupts()
#define  port_clr_lock()            enableInterrupts()

#define  port_sys_lock()       do { char __LOCK = port_get_lock(); port_set_lock()
#define  port_sys_unlock()          port_put_lock(__LOCK); } while(0)

#define  port_isr_lock()       do { port_set_lock()
#define  port_isr_unlock()          port_clr_lock(); } while(0)

/* -------------------------------------------------------------------------- */

__STATIC_INLINE
void port_ctx_switch( void )
{
	TIM3->EGR = TIM3_EGR_CC1G;
}

/* -------------------------------------------------------------------------- */

__STATIC_INLINE
void port_ctx_switchNow( void )
{
	port_ctx_switch();
	port_clr_lock();
}

/* -------------------------------------------------------------------------- */

__STATIC_INLINE
void port_ctx_switchLock( void )
{
	port_ctx_switchNow();
	port_set_lock();
}

/* -------------------------------------------------------------------------- */

__STATIC_INLINE
void port_ctx_reset( void )
{
	TIM3->SR1 = (uint8_t) ~TIM3_SR1_CC1IF;
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
void port_tmr_start( unsigned timeout )
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
