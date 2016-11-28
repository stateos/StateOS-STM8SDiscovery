#ifndef __LED_H
#define __LED_H

#include <stm8s.h>
#include <bitfield.h>

#define LED BIT(GPIOD->ODR, 0)

static inline
void LED_Config( void )
{
	GPIOD->DDR |= 0x01;
	GPIOD->CR1 |= 0x01;
	GPIOD->ODR |= 0x01;
}

static inline
void LED_Tick( void )
{
	LED ^= 1;
}

#endif//__LED_H
