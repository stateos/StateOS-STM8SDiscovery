/******************************************************************************

    @file    StateOS: oscore.c
    @author  Rajmund Szymanski
    @date    08.08.2017
    @brief   StateOS port file for STM8 uC.

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

#if defined(__CSMC__)

#include <oskernel.h>

/* -------------------------------------------------------------------------- */

void core_tsk_flip( void *sp )
{
	_set_SP((char *)sp - 1);
	core_tsk_loop();
}

/* -------------------------------------------------------------------------- */

#endif // __CSMC__
