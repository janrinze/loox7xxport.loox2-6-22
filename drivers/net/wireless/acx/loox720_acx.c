/*
 * WLAN (TI TNETW1100B) support in the hx470x.
 *
 * Copyright (c) 2006 SDG Systems, LLC
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 * 28-March-2006          Todd Blumer <todd@sdgsystems.com>
 */


#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/leds.h>

#include <asm/hardware.h>

#include <asm/arch/pxa-regs.h>
#include <asm/arch/loox720.h>
#include <asm/arch/loox720-cpld.h>
#include <asm/io.h>

#include "acx_hw.h"

static int
loox720_wlan_start( void )
{
	loox720_egpio_set_bit(LOOX720_CPLD_WIFI_POWER, 1);
	loox720_enable_led(LOOX720_LED_LEFT, LOOX720_LED_COLOR_A | LOOX720_LED_BLINK);
	return 0;
}

static int
loox720_wlan_stop( void )
{
	loox720_disable_led(LOOX720_LED_LEFT, LOOX720_LED_COLOR_A);
	loox720_egpio_set_bit(LOOX720_CPLD_WIFI_POWER, 0);
	return 0;
}

static struct acx_hardware_data acx_data = {
	.start_hw	= loox720_wlan_start,
	.stop_hw	= loox720_wlan_stop,
};

static struct platform_device acx_device = {
	.name	= "acx-mem",
	.dev	= {
		.platform_data = &acx_data,
	},
};

static int __init
loox720_wlan_init( void )
{
	printk( "loox720_wlan_init: acx-mem platform_device_register\n" );
	return platform_device_register( &acx_device );
}


static void __exit
loox720_wlan_exit( void )
{
	platform_device_unregister( &acx_device );
}

module_init( loox720_wlan_init );
module_exit( loox720_wlan_exit );

MODULE_AUTHOR( "Tomasz Figa <tom3q@staszic.waw.pl>" );
MODULE_DESCRIPTION( "WLAN driver for Loox 720" );
MODULE_LICENSE( "GPL" );

