/*
 * WLAN (TI TNETW1100B) support in the HTC Blueangel.
 *
 * Copyright (c) 2006 SDG Systems, LLC
 * Copyright (c) 2006 Paul Sokolovsky
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 * Based on hx4700_acx.c
 */


#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

#include <asm/hardware.h>

#include <asm/arch/pxa-regs.h>
#include <asm/arch/htcblueangel-gpio.h>
#include <asm/arch/htcblueangel-asic.h>
#include <linux/soc/asic3_base.h>
//#include <asm/arch/ipaq-asic3.h>
#include <asm/io.h>
#include "acx_hw.h"

// Mem area of PCMCIA socket #0
#define WLAN_BASE	0x2c000000

extern struct platform_device blueangel_asic3;

static int
htcblueangel_wlan_start( void )
{
	printk("htcblueangel_wlan_start\n");
        asic3_set_gpio_out_a(&blueangel_asic3.dev, 1 << GPIOA_WLAN_PWR1, 1 << GPIOA_WLAN_PWR1);
        asic3_set_gpio_out_a(&blueangel_asic3.dev, 1 << GPIOA_WLAN_PWR2, 1 << GPIOA_WLAN_PWR2);
        asic3_set_gpio_out_b(&blueangel_asic3.dev, 1 << GPIOB_WLAN_PWR3, 1 << GPIOB_WLAN_PWR3);
        mdelay(100);

        asic3_set_gpio_out_a(&blueangel_asic3.dev, 1 << GPIOA_WLAN_RESET, 1 << GPIOA_WLAN_RESET);
        mdelay(100);
        asic3_set_gpio_out_a(&blueangel_asic3.dev, 1 << GPIOA_WLAN_RESET, 0);
        mdelay(100);

	return 0;
}

static int
htcblueangel_wlan_stop( void )
{
	printk("blueangel_wlan_stop\n");

        asic3_set_gpio_out_a(&blueangel_asic3.dev, 1 << GPIOA_WLAN_PWR1, 0);
        asic3_set_gpio_out_a(&blueangel_asic3.dev, 1 << GPIOA_WLAN_PWR2, 0);
        asic3_set_gpio_out_b(&blueangel_asic3.dev, 1 << GPIOB_WLAN_PWR3, 0);
	return 0;
}

static struct resource acx_resources[] = {
	[0] = {
		.start	= WLAN_BASE,
		.end	= WLAN_BASE + 0x20,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= BLUEANGEL_IRQ(WLAN_IRQ_N),
		.end	= BLUEANGEL_IRQ(WLAN_IRQ_N),
		.flags	= IORESOURCE_IRQ,
	},
};

static struct acx_hardware_data acx_data = {
	.start_hw	= htcblueangel_wlan_start,
	.stop_hw	= htcblueangel_wlan_stop,
};

static struct platform_device acx_device = {
	.name	= "acx-mem",
	.dev	= {
		.platform_data = &acx_data,
	},
	.num_resources	= ARRAY_SIZE( acx_resources ),
	.resource	= acx_resources,
};

static int __init
htcblueangel_wlan_init( void )
{
	printk( "htcblueangel_wlan_init: acx-mem platform_device_register\n" );
	return platform_device_register( &acx_device );
}


static void __exit
htcblueangel_wlan_exit( void )
{
	platform_device_unregister( &acx_device );
}

module_init( htcblueangel_wlan_init );
module_exit( htcblueangel_wlan_exit );

MODULE_AUTHOR( "Paul Sokolovsky <pfalcon@handhelds.org>" );
MODULE_DESCRIPTION( "WLAN driver for HTC Blueangel" );
MODULE_LICENSE( "GPL" );
