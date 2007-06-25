/*
 * LEDs support for the HTC Blueangel
 *
 * Copyright (c) 2006  Anton Vorontsov <cbou@mail.ru>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/soc/asic3_base.h>

#include <asm/hardware/ipaq-asic3.h>
#include <asm/mach-types.h>
#include <asm/hardware/asic3_leds.h>
#include <asm/arch/htcblueangel-asic.h>

DEFINE_LED_TRIGGER_SHARED_GLOBAL(blueangel_radio_trig);
EXPORT_LED_TRIGGER_SHARED(blueangel_radio_trig);

static struct asic3_led blueangel_leds[] = {
	{
		.led_cdev  = {
 			.name	         = "blueangel:red",
 			.default_trigger = "ds2760-battery.0-charging-or-full",
		},
		.hw_num = 0,

	},
	{
		.led_cdev  = {
			.name	         = "blueangel:green",
 			.default_trigger = "ds2760-battery.0-full",
		},
		.hw_num = 1,
	},
	{
		.led_cdev  = {
			.name	         = "blueangel:phonebuttons",
			.default_trigger = "blueangel-phonebuttons",
		},
		.hw_num = -1,
		.gpio_num = ('B'-'A')*16+GPIOB_PHONEL_PWR_ON,
	},
	{
		.led_cdev  = {
			.name	         = "blueangel:vibra",
			.default_trigger = "blueangel-vibra",
		},
		.hw_num = -1,
		.gpio_num = ('B'-'A')*16+GPIOB_VIBRA_PWR_ON,
	},
	{
		.led_cdev  = {
			.name	         = "blueangel:kbdbacklight",
			.default_trigger = "blueangel-kbdbacklight",
		},
		.hw_num = -1,
		.gpio_num = ('C'-'A')*16+GPIOC_KEYBL_PWR_ON,
	},
};

void blueangel_leds_release(struct device *dev)
{
	return;
}

static
struct asic3_leds_machinfo blueangel_leds_machinfo = {
	.num_leds = ARRAY_SIZE(blueangel_leds),
	.leds = blueangel_leds,
	.asic3_pdev = &blueangel_asic3,
};

static
struct platform_device blueangel_leds_pdev = {
	.name = "asic3-leds",
	.dev = {
		.platform_data = &blueangel_leds_machinfo,
		.release = blueangel_leds_release,
	},
};

static
int __init blueangel_leds_init(void)
{
	int ret;
	printk("HTC Blueangel ASIC3 LEDs Driver\n");
	led_trigger_register_shared("blueangel-radio", &blueangel_radio_trig);
	
	ret = asic3_leds_register();
	if (ret) goto asic3_leds_failed;

	ret = platform_device_register(&blueangel_leds_pdev);
	if (ret) goto platform_device_failed;
	
	goto success;

platform_device_failed:
	asic3_leds_unregister();
asic3_leds_failed:
	led_trigger_unregister_shared(blueangel_radio_trig);
	printk("blueangel ASIC3 LEDs Driver failed to init");
success:
	return ret;
}

static
void __exit blueangel_leds_exit(void)
{
	led_trigger_unregister_shared(blueangel_radio_trig);
	platform_device_unregister(&blueangel_leds_pdev);
	asic3_leds_unregister();
	return;
}

module_init(blueangel_leds_init);
module_exit(blueangel_leds_exit);

MODULE_AUTHOR("Anton Vorontsov <cbou@mail.ru>");
MODULE_DESCRIPTION("HTC Blueangel LEDs driver");
MODULE_LICENSE("GPL");
