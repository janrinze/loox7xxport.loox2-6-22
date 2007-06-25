/* 
 * LEDs device driver for the HP iPaq H5400
 *
 * Copyright (c) 2007  Anton Vorontsov <cbou@mail.ru>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <asm/hardware/samcop_leds.h>
#include <asm/arch/h5400-asic.h>

DEFINE_LED_TRIGGER_SHARED_GLOBAL(h5400_radio_trig);
EXPORT_SYMBOL(h5400_radio_trig);

static struct samcop_led h5400_leds[] = {
	{
		.led_cdev = {
			.name = "h5400:red-right",
			.default_trigger = "ds2760-battery.0-charging",
			.flags = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 0,
	},
	{
		.led_cdev = {
			.name = "h5400:green-right",
			.default_trigger = "ds2760-battery.0-full",
			.flags = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 1,
	},
	{
		.led_cdev = {
			.name = "h5400:red-left",
			.flags = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 2,
	},
	{
		.led_cdev = {
			.name = "h5400:green-left",
			.flags = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 3,
	},
	{
		.led_cdev = {
			.name = "h5400:blue",
			.default_trigger = "h5400-radio",
			.flags = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 4,
	},
};

static struct samcop_leds_machinfo h5400_leds_info = {
	.leds = h5400_leds,
	.num_leds = 5,
	.samcop_pdev = &h5400_samcop,
};

void h5400_leds_null_release(struct device *dev)
{
	return;
}

static struct platform_device samcop_leds_pdev = {
	.name = "samcop-leds",
	.id = -1,
	.dev = {
		.platform_data = &h5400_leds_info,
		.release = h5400_leds_null_release,
	},
};

int h5400_leds_init(void)
{
	int ret = 0;
	
	led_trigger_register_shared("h5400-radio", &h5400_radio_trig);

	ret = samcop_leds_register();

	if (ret) {
		printk("%s: failed to register samcop-leds\n", __FUNCTION__);
		goto samcop_leds_failed;
	}

	ret = platform_device_register(&samcop_leds_pdev);
	if (ret) {
		printk("%s: failed to register platform device\n",
		       __FUNCTION__);
		goto pdev_failed;
	}

	goto success;

pdev_failed:
	samcop_leds_unregister();
samcop_leds_failed:
	led_trigger_unregister_shared(h5400_radio_trig);
success:
	return ret;
}

void h5400_leds_exit(void)
{
	platform_device_unregister(&samcop_leds_pdev);
	samcop_leds_unregister();
	led_trigger_unregister_shared(h5400_radio_trig);
	return;
}

module_init(h5400_leds_init);
module_exit(h5400_leds_exit);

MODULE_AUTHOR("Anton Vorontsov <cbou@mail.ru>");
MODULE_LICENSE("GPL");
