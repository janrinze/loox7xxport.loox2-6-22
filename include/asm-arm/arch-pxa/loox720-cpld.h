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

#define BIT_POS(x)	(x/32)
#define BIT_SHIFT(x)	(x&31)
#define BIT_MSK(x)	(u32)(1<<BIT_SHIFT(x))

//#define	LOOX720_CPLD_LCD_COLOR_BIT	80
//#define	LOOX720_CPLD_LCD_EN_BIT		81

//#define	LOOX720_CPLD_RS232_EN_BIT	83

//#define	LOOX720_CPLD_SD_EN		113

#define LOOX720_LED1_COLOR_BLUE	 1
#define LOOX720_LED1_COLOR_GREEN 2
#define LOOX720_LED1_BLINK	 4
#define LOOX720_LED1_BLINK2	 8

#define LOOX720_LED2_COLOR_GREEN  16
#define LOOX720_LED2_COLOR_RED    32
#define LOOX720_LED2_COLOR_ORANGE 64
#define LOOX720_LED2_BLINK	  128
#define LOOX720_LED2_BLINK2	  256

#define LOOX720_CPLD_USB_PULLUP_BIT	145

#define LOOX720_CPLD_LCD_BIT		147

#define LOOX720_CPLD_SOUND_BIT		208

#define LOOX720_CPLD_SD_BIT		225

#define LOOX720_CPLD_BACKLIGHT_BIT	242

#define LOOX720_CPLD_BT_ENABLE_BIT_A	44

#define LOOX720_CPLD_BT_ENABLE_BIT_B	60

extern void	loox720_set_leds(int mode);
extern void	loox720_egpio_set_bit(int bit, int val);

#endif
