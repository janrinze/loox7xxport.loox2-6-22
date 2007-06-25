/*
 *
 * Definitions for HP iPAQ H22xx Handheld Computer
 *
 * Copyright 2003 Hewlett-Packard Company.
 *
 * Use consistent with the GNU GPL is permitted,
 * provided that this copyright notice is
 * preserved in its entirety in all copies and derived works.
 *
 * COMPAQ COMPUTER CORPORATION MAKES NO WARRANTIES, EXPRESSED OR IMPLIED,
 * AS TO THE USEFULNESS OR CORRECTNESS OF THIS CODE OR ITS
 * FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 * Author: Jamey Hicks
 *
 * Names and values copied from the H2200 Asic Companion specifications:
 * http://www.handhelds.org/platforms/hp/ipaq-h22xx/
 * 
 * History:
 * 
 * See http://cvs.handhelds.org/cgi-bin/viewcvs.cgi/linux/kernel26/include/asm-arm/arch-pxa/h2200-asic.h
 */

#ifndef _INCLUDE_H2200_ASIC_H_ 
#define _INCLUDE_H2200_ASIC_H_

#define H2200_HAMCOP_BASE	PXA_CS0_PHYS /* from 02-cotulla_IF.pdf */

extern struct platform_device h2200_hamcop;

extern void (*h2200_led_hook)(struct device *dev, int led_num, int duty_time,
	     int cycle_time);
extern void h2200_set_led(int led_num, int duty_time, int cycle_time);

extern u8 * get_hamcop_bootloader(void);

#endif /*  _INCLUDE_H2200_ASIC_H_ */
