/******************************************************************************

    @file    StateOS: oscore.h
    @author  Rajmund Szymanski
    @date    26.03.2017
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

#ifndef __STATEOSCORE_H
#define __STATEOSCORE_H

#include <osbase.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

// task context

typedef struct __ctx ctx_t;

struct __ctx
{
	char     dummy; // sp is a pointer to the first free byte on the stack
	// context saved by the software
	char     c_lreg[4];
	char     c_y[3];
	char     c_x[3];
	// context saved by the hardware
	char     cc;
	char     a;
	unsigned x, y;
FAR	void   * pc;
};

#define _CTX_INIT( pc ) { 0, { 0 }, { 0 }, { 0 }, 0x20, 0, 0, 0, (FAR void *) pc }

/* -------------------------------------------------------------------------- */

__STATIC_INLINE
void port_ctx_init( ctx_t *ctx, fun_t *pc )
{
	ctx->cc = 0x20;
	ctx->pc = (FAR void *) pc;
}

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif//__STATEOSCORE_H
