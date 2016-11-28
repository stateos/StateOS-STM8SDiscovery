/******************************************************************************
 * @file    bitfield.h
 * @author  Rajmund Szymanski
 ******************************************************************************/

#ifndef __BITFIELD_H
#define __BITFIELD_H
#ifndef __cplusplus

/* -------------------------------------------------------------------------- */

// m³odsze bity
#define __CTZ( msk )         \
	((((msk)&0xFF)==0) ? 8 : \
	 (((msk)&0x7F)==0) ? 7 : \
	 (((msk)&0x3F)==0) ? 6 : \
	 (((msk)&0x1F)==0) ? 5 : \
	 (((msk)&0x0F)==0) ? 4 : \
	 (((msk)&0x07)==0) ? 3 : \
	 (((msk)&0x03)==0) ? 2 : \
	 (((msk)&0x01)==0) ? 1 : 0)

/* -------------------------------------------------------------------------- */

// starsze bity
#define __CLZ( msk )         \
	((((msk)&0xFF)==0) ? 8 : \
	 (((msk)&0xFE)==0) ? 7 : \
	 (((msk)&0xFC)==0) ? 6 : \
	 (((msk)&0xF8)==0) ? 5 : \
	 (((msk)&0xF0)==0) ? 4 : \
	 (((msk)&0xE0)==0) ? 3 : \
	 (((msk)&0xC0)==0) ? 2 : \
	 (((msk)&0x80)==0) ? 1 : 0)

/* -------------------------------------------------------------------------- */

// szerokoœæ pola bitowego
#define __CNT( msk )  (8-__CLZ(((msk)&0xFF)>>__CTZ(msk)))

/* -------------------------------------------------------------------------- */

#define BF(var, msk)  ((*(struct { unsigned char  :__CTZ(msk); \
                          volatile unsigned char f:__CNT(msk); \
                                   unsigned char  :__CLZ(msk); }*)&(var)).f)

/* -------------------------------------------------------------------------- */

#define BIT(var, bit) ((*(struct { unsigned char  :     (bit); \
                          volatile unsigned char f: 1        ; \
                                   unsigned char  : 7 - (bit); }*)&(var)).f)

/* -------------------------------------------------------------------------- */

#endif//__cplusplus
#endif//__BITFIELD_H
