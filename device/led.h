#ifndef __LED_H__
#define __LED_H__

// led: GPIOD.0

#include <stm8s.h>

struct  __LEDs { volatile uint8_t f: 1; uint8_t: 7; };

#define   LED (((struct __LEDs *)&(GPIOD->ODR))->f)

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
