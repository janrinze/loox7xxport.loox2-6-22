/*
 * include/asm/arm/arch-pxa/loox720-cpld.h
 *
 * Authors: Piotr Czechowicz <p.czechowicz@admitech.pl>
 *
 */

#ifndef _LOOX720_CPLD_H_
#define _LOOX720_CPLD_H_

#include <asm/arch/pxa-regs.h>

#define LOOX720_CPLD_PHYS	PXA_CS4_PHYS
#define LOOX720_CPLD_SIZE	32

#define LOOX720_CPLD_USB_PULLUP_BIT	145
//#define LOOX720_CPLD_BACKLIGHT_BIT 0x0004
#define LOOX720_CPLD_BACKLIGHT_BIT	242

extern void	loox720_egpio_set_bit(int bit, int val);

#endif
