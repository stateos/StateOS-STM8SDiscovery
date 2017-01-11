/******************************************************************************

    @file    StateOS: os_flg.c
    @author  Rajmund Szymanski
    @date    10.01.2017
    @brief   This file provides set of functions for StateOS.

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

#include <os.h>

/* -------------------------------------------------------------------------- */
flg_t *flg_create( void )
/* -------------------------------------------------------------------------- */
{
	flg_t *flg;

	port_sys_lock();

	flg = core_sys_alloc(sizeof(flg_t));

	port_sys_unlock();

	return flg;
}

/* -------------------------------------------------------------------------- */
void flg_kill( flg_t *flg )
/* -------------------------------------------------------------------------- */
{
	assert(flg);

	port_sys_lock();

	core_all_wakeup(flg, E_STOPPED);

	port_sys_unlock();
}

/* -------------------------------------------------------------------------- */
static
unsigned priv_flg_wait( flg_t *flg, unsigned flags, unsigned mode, unsigned time, unsigned(*wait)() )
/* -------------------------------------------------------------------------- */
{
	unsigned event = E_SUCCESS;

	assert(flg);

	port_sys_lock();

	tsk_t *cur = Current;

	cur->mode   =  mode & flgAll;
	cur->flags  = (mode & flgIgnore) ? flags : (flags & ~flg->flags);
	flg->flags &= ~flags;

	if (cur->flags && ((cur->mode != flgOne) || (cur->flags == flags)))
	event = wait(flg, time);

	port_sys_unlock();

	return event;
}

/* -------------------------------------------------------------------------- */
unsigned flg_waitUntil( flg_t *flg, unsigned flags, unsigned mode, unsigned time )
/* -------------------------------------------------------------------------- */
{
	return priv_flg_wait(flg, flags, mode, time, core_tsk_waitUntil);
}

/* -------------------------------------------------------------------------- */
unsigned flg_waitFor( flg_t *flg, unsigned flags, unsigned mode, unsigned delay )
/* -------------------------------------------------------------------------- */
{
	return priv_flg_wait(flg, flags, mode, delay, core_tsk_waitFor);
}

/* -------------------------------------------------------------------------- */
void flg_give( flg_t *flg, unsigned flags )
/* -------------------------------------------------------------------------- */
{
	tsk_t *tsk;
	
	assert(flg);

	port_sys_lock();

	flags = flg->flags |= flags;

	for (tsk = flg->queue; tsk; tsk = tsk->obj.queue)
	{
		if (tsk->flags & flags)
		{
			flg->flags &= ~tsk->flags;
			tsk->flags &= ~flags;
			if (tsk->flags && (tsk->mode != flgOne)) continue;
			core_one_wakeup(tsk = tsk->back, E_SUCCESS);
		}
	}

	port_sys_unlock();
}

/* -------------------------------------------------------------------------- */
