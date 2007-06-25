/*
 * LEDs support for the HP iPaq h4000
 *
 * Copyright (c) 2006  Anton Vorontsov <cbou@mail.ru>
 * Copyright (c) 2006  Paul Sokolovsky <pmiscml@gmail.com>
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

#include <asm/hardware/ipaq-asic3.h>
#include <linux/soc/asic3_base.h>
#include <asm/mach-types.h>
#include <asm/arch/ipaq.h>
#include <asm/hardware/asic3_leds.h>

DEFINE_LED_TRIGGER_SHARED_GLOBAL(h4000_radio_trig);
EXPORT_LED_TRIGGER_SHARED(h4000_radio_trig);

extern struct platform_device h4000_asic3;

static struct asic3_led h4000_leds[] = {
	{
		.led_cdev  = {
			.name	         = "h4000:red",
			.default_trigger = "main-battery-charging",
			.flags = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 0,
	},
	{
		.led_cdev  = {
			.name	         = "h4000:right-green",
			.default_trigger = "main-battery-full",
			.flags = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 1,
	},
	{
		.led_cdev  = {
			.name	         = "h4000:left-green",
			.default_trigger = "h4000-wifi",
		},
		.hw_num = -1,
		.gpio_num = 4,
	},
	{
		.led_cdev  = {
			.name	         = "h4000:blue",
			.default_trigger = "bluetooth",
		},
		.hw_num = -1,
		.gpio_num = 5,
	},
};

void h4000_leds_release(struct device *dev)
{
	return;
}

static
struct asic3_leds_machinfo h4000_leds_machinfo = {
	.num_leds = ARRAY_SIZE(h4000_leds),
	.leds = h4000_leds,
	.asic3_pdev = &h4000_asic3,
};

static
struct platform_device h4000_leds_pdev = {
	.name = "asic3-leds",
	.dev = {
		.platform_data = &h4000_leds_machinfo,
		.release = h4000_leds_release,
	},
};

static
int __init h4000_leds_init(void)
{
	int ret;
	printk("h4000 LEDs Driver\n");
	led_trigger_register_shared("h4000-radio", &h4000_radio_trig);
	
	ret = asic3_leds_register();
	if (ret) goto asic3_leds_failed;

	ret = platform_device_register(&h4000_leds_pdev);
	if (ret) goto platform_device_failed;
	
	goto success;

platform_device_failed:
	asic3_leds_unregister();
asic3_leds_failed:
	led_trigger_unregister_shared(h4000_radio_trig);
	printk("h4000 LEDs Driver failed to init");
success:
	return ret;
}

static
void __exit h4000_leds_exit(void)
{
	led_trigger_unregister_shared(h4000_radio_trig);
	platform_device_unregister(&h4000_leds_pdev);
	asic3_leds_unregister();
	return;
}

module_init(h4000_leds_init);
module_exit(h4000_leds_exit);

MODULE_AUTHOR("Paul Sokolovsky <pmiscml@gmail.com>");
MODULE_DESCRIPTION("HP iPAQ h4000 LEDs driver");
MODULE_LICENSE("GPL");
