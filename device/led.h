#ifndef __LED_H__
#define __LED_H__

#include <stm8s.h>
#include <bitfield.h>

// led: GPIOD.0

#define   LED  BIT(GPIOD->ODR, 0)

static inline void led_init( void )
{
	GPIOD->DDR |= 0x01;
	GPIOD->CR1 |= 0x01;
	GPIOD->ODR |= 0x01;
}

static inline void led_set   ( void ) { GPIOD->ODR &= 0xFE; }
static inline void led_clear ( void ) { GPIOD->ODR |= 0x01; }
static inline void led_toggle( void ) { GPIOD->ODR ^= 0x01; }

#endif//__LED_H__
