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

#define LOOX720_LED1_BLUE		1
#define LOOX720_LED1_GREEN		2
#define LOOX720_LED1_BLINK		4
#define LOOX720_LED1_BLINK2		8

#define LOOX720_LED2_GREEN  	16
#define LOOX720_LED2_ORANGE 	32
#define LOOX720_LED2_BLINK	 	64
#define LOOX720_LED2_BLINK2	  	128
#define LOOX720_LED2_RED		48

#define LOOX720_CPLD_LED1_BIT_A 256
#define LOOX720_CPLD_LED1_BIT_B 512
#define LOOX720_CPLD_LED1_BIT_C 1024
#define LOOX720_CPLD_LED1_BIT_D 2048
#define LOOX720_CPLD_LED1_BLINK 32768

#define LOOX720_CPLD_LED2_BIT_A 16
#define LOOX720_CPLD_LED2_BIT_B 32
#define LOOX720_CPLD_LED2_BIT_C 64
#define LOOX720_CPLD_LED2_BIT_D 128
#define LOOX720_CPLD_LED2_BLINK 16384

#define LOOX720_CPLD_USB_PULLUP_BIT	145

#define LOOX720_CPLD_LCD_CONSOLE_BIT	146	/* If it's low, then FB console doesn't show up */
#define LOOX720_CPLD_LCD_BIT		147

#define LOOX720_CPLD_LCD_COLOR_BIT	160
#define LOOX720_CPLD_LCD_BIT2		161
#define LOOX720_CPLD_SERIAL_BIT		163

#define LOOX720_CPLD_BLUETOOTH_POWER	192
#define LOOX720_CPLD_BLUETOOTH_RADIO	210
#define LOOX720_CPLD_CAMERA_FLASH_BIT	195

#define LOOX720_CPLD_SOUND_BIT		208

#define LOOX720_CPLD_WIFI_POWER		211

#define LOOX720_CPLD_SD_BIT		225

#define LOOX720_CPLD_BACKLIGHT_BIT	242

#define LOOX720_CPLD_STATUS_BIT_A	44
#define LOOX720_CPLD_STATUS_BIT_B	60
#define LOOX720_CPLD_STATUS_BIT_C	36
#define LOOX720_CPLD_STATUS_BIT_D	39

extern void	loox720_set_leds(int mode);
extern void	loox720_egpio_set_bit(int bit, int val);
extern u32 loox720_cpld_reg_read(int regno);

#endif
