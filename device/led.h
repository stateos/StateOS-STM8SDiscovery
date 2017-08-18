#ifndef __LED_H__
#define __LED_H__

// led: GPIOD.0

#include <stm8s.h>

struct  __LEDs { uint8_t: 7; volatile uint8_t f: 1; };

#define   LED (((struct __LEDs *)&(GPIOD->ODR))->f)

static inline void led_init( void )
{
	GPIOD->DDR |= 0x01;
	GPIOD->CR1 |= 0x01;
	LED = 1;
}

static inline void led_set   ( void ) { LED  = 0; }
static inline void led_clear ( void ) { LED  = 1; }
static inline void led_toggle( void ) { LED ^= 1; }

#endif//__LED_H__
