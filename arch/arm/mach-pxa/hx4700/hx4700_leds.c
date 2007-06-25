/*
 * LEDs support for the HP iPaq hx4700
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

#include <asm/hardware/ipaq-asic3.h>
#include <linux/soc/asic3_base.h>
#include <asm/mach-types.h>
#include <asm/arch/ipaq.h>
#include <asm/hardware/asic3_leds.h>

DEFINE_LED_TRIGGER_SHARED_GLOBAL(hx4700_radio_trig);
EXPORT_LED_TRIGGER_SHARED(hx4700_radio_trig);

extern struct platform_device hx4700_asic3;

static struct asic3_led hx4700_leds[] = {
	{
		.led_cdev  = {
			.name	         = "hx4700:amber",
			.default_trigger = "ds2760-battery.0-charging",
			.flags = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 0,

	},
	{
		.led_cdev  = {
			.name	         = "hx4700:green",
			.default_trigger = "ds2760-battery.0-full",
			.flags = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 1,
	},
	{
		.led_cdev  = {
			.name	         = "hx4700:blue",
			.default_trigger = "hx4700-radio",
			.flags = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 2,
	},
};

void hx4700_leds_release(struct device *dev)
{
	return;
}

static
struct asic3_leds_machinfo hx4700_leds_machinfo = {
	.num_leds = ARRAY_SIZE(hx4700_leds),
	.leds = hx4700_leds,
	.asic3_pdev = &hx4700_asic3,
};

static
struct platform_device hx4700_leds_pdev = {
	.name = "asic3-leds",
	.dev = {
		.platform_data = &hx4700_leds_machinfo,
		.release = hx4700_leds_release,
	},
};

static
int __init hx4700_leds_init(void)
{
	int ret;
	printk("hx4700 LEDs Driver\n");
	led_trigger_register_shared("hx4700-radio", &hx4700_radio_trig);
	
	ret = asic3_leds_register();
	if (ret) goto asic3_leds_failed;

	ret = platform_device_register(&hx4700_leds_pdev);
	if (ret) goto platform_device_failed;
	
	goto success;

platform_device_failed:
	asic3_leds_unregister();
asic3_leds_failed:
	led_trigger_unregister_shared(hx4700_radio_trig);
	printk("hx4700 LEDs Driver failed to init");
success:
	return ret;
}

static
void __exit hx4700_leds_exit(void)
{
	led_trigger_unregister_shared(hx4700_radio_trig);
	platform_device_unregister(&hx4700_leds_pdev);
	asic3_leds_unregister();
	return;
}

module_init(hx4700_leds_init);
module_exit(hx4700_leds_exit);

MODULE_AUTHOR("Anton Vorontsov <cbou@mail.ru>");
MODULE_DESCRIPTION("HP iPAQ hx4700 LEDs driver");
MODULE_LICENSE("GPL");
