/*
 * LCD driver for Fujitsu-Siemens Loox 720
 *
 * Authors: ...
 *
 * based on previous work, see below:
 *
 * Machine initialization for Dell Axim X3
 *
 * Authors: Andrew Zabolotny <zap@homelink.ru>
 *
 * For now this is mostly a placeholder file; since I don't own an Axim X3
 * it is supposed the project to be overtaken by somebody else. The code
 * in here is *supposed* to work so that you can at least boot to the command
 * line, but there is no guarantee.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/lcd.h>
#include <linux/backlight.h>
#include <linux/err.h>

#include <asm/arch/aximx3-init.h>
#include <asm/arch/aximx3-gpio.h>
#include <asm/arch/loox720-gpio.h>
#include <linux/fb.h>
#include <asm/mach-types.h>
#include "asm/arch/pxa-regs.h"
#include "asm/arch/pxafb.h"
#include <linux/platform_device.h>
#include <linux/corgi_bl.h>


static struct pxafb_mode_info loox720_lcd_mode_info = {
	.pixclock		= 57692,
	.bpp			= 16,
	.xres			= 480,
	.yres			= 640,
	.hsync_len		= 4,
	.left_margin		= 20,
	.right_margin		= 8,
	.vsync_len		= 1,
	.upper_margin		= 8,
	.lower_margin		= 9,
	.sync			= 0,
};

static struct pxafb_mach_info loox720_fb_info =
{
	.modes		= &loox720_lcd_mode_info,
	.num_modes	= 1,
	.lccr0		= LCCR0_Act | LCCR0_Sngl | LCCR0_Color,
	.lccr3		= LCCR3_OutEnH | LCCR3_PixRsEdg,
};

static int loox720_lcd_set_power(struct lcd_device *dev, int power)
{
    return 0;
}

static int loox720_lcd_get_power(struct lcd_device *dev)
{
    return 0;
}

static struct lcd_ops loox720_lcd_properties =
{
	.set_power = loox720_lcd_set_power,
	.get_power = loox720_lcd_get_power,
	/* @@ more here @@ */
};

static struct lcd_device *pxafb_lcd_device;

static int __init
loox720_lcd_init (void)
{
	if (! machine_is_loox720 ())
		return -ENODEV;

	set_pxa_fb_info(&loox720_fb_info);
	pxafb_lcd_device = lcd_device_register("pxafb", NULL, &loox720_lcd_properties);

//	if (IS_ERR (pxafb_lcd_device))
//		return PTR_ERR (pxafb_lcd_device);
	//possible BUG location, dunno what to pass as 2nd param
//	pxafb_backlight_device = backlight_device_register("pxafb",
//		pxafb_lcd_device->class_dev.dev, NULL,
//		&loox720_backlight_properties);
//	if (IS_ERR (pxafb_backlight_device)) {
//		lcd_device_unregister (pxafb_lcd_device);
//		return PTR_ERR (pxafb_backlight_device);
//	}

	return 0;
}

static void __exit
loox720_lcd_exit (void)
{
	lcd_device_unregister (pxafb_lcd_device);
}

module_init (loox720_lcd_init);
module_exit (loox720_lcd_exit);

MODULE_AUTHOR("...");
MODULE_DESCRIPTION("Loox 720 Core frambuffer driver");
MODULE_LICENSE("GPL");

