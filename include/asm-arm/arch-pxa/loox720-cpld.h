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
#define LOOX720_CPLD_SIZE	0x00100000

#define LOOX720_CPLD_BACKLIGHT_BIT 0x0004

void loox720_cpld_enable( u16 bits );
void loox720_cpld_disable( u16 bits );

#endif
