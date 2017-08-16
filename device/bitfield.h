/******************************************************************************
 * @file    bitfield.h
 * @author  Rajmund Szymanski
 ******************************************************************************/

#ifndef __BITFIELD_H
#define __BITFIELD_H
#ifndef __cplusplus

/* -------------------------------------------------------------------------- */

#define __bit_ctz01( mask ) ((((mask) << 31) == 0) ? 1 : 0)
#define __bit_ctz02( mask ) ((((mask) << 31) == 0) ? ( 1 + __bit_ctz01((mask) >>  1)) : __bit_ctz01(mask))
#define __bit_ctz04( mask ) ((((mask) << 30) == 0) ? ( 2 + __bit_ctz02((mask) >>  2)) : __bit_ctz02(mask))
#define __bit_ctz08( mask ) ((((mask) << 28) == 0) ? ( 4 + __bit_ctz04((mask) >>  4)) : __bit_ctz04(mask))
#define __bit_ctz16( mask ) ((((mask) << 24) == 0) ? ( 8 + __bit_ctz08((mask) >>  8)) : __bit_ctz08(mask))
#define __bit_ctz32( mask ) ((((mask) << 16) == 0) ? (16 + __bit_ctz16((mask) >> 16)) : __bit_ctz16(mask))

#define __bit_clz01( mask ) ((((mask) >> 31) == 0) ? 1 : 0)
#define __bit_clz02( mask ) ((((mask) >> 31) == 0) ? ( 1 + __bit_clz01((mask) <<  1)) : __bit_clz01(mask))
#define __bit_clz04( mask ) ((((mask) >> 30) == 0) ? ( 2 + __bit_clz02((mask) <<  2)) : __bit_clz02(mask))
#define __bit_clz08( mask ) ((((mask) >> 28) == 0) ? ( 4 + __bit_clz04((mask) <<  4)) : __bit_clz04(mask))
#define __bit_clz16( mask ) ((((mask) >> 24) == 0) ? ( 8 + __bit_clz08((mask) <<  8)) : __bit_clz08(mask))
#define __bit_clz32( mask ) ((((mask) >> 16) == 0) ? (16 + __bit_clz16((mask) << 16)) : __bit_clz16(mask))

/* -------------------------------------------------------------------------- */

#define __CTZ( mask )            __bit_ctz08(((uint32_t)(mask)      ) | 0xFFFFFF00)
#define __CLZ( mask )            __bit_clz08(((uint32_t)(mask) << 24) | 0x00FFFFFF)
#define __CNT( mask )       (8 - __CLZ(((mask) & 0xFF) >> __CTZ(mask)))

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
