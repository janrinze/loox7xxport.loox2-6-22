/*
 * Copyright 2006 Roman Moravcik <roman.moravcik@gmail.com>
 *
 * LED driver for HP iPAQ RX3000
 *
 * Based on hx4700_leds.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/soc/asic3_base.h>

#include <asm/hardware.h>

#include <asm/arch/rx3000.h>
#include <asm/arch/rx3000-asic3.h>

#include <asm/hardware/asic3_leds.h>

DEFINE_LED_TRIGGER_SHARED_GLOBAL(rx3000_radio_trig);
EXPORT_LED_TRIGGER_SHARED(rx3000_radio_trig);

extern struct platform_device s3c_device_asic3;

static struct asic3_led rx3000_leds[] = {
	{
		.led_cdev  = {
			.name	         = "rx3000:green",
			.default_trigger = "ds2760-battery.0-full",
			.flags		 = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 0,

	},
	{
		.led_cdev  = {
			.name	         = "rx3000:red",
			.default_trigger = "ds2760-battery.0-charging",
			.flags		 = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 1,
	},
	{
		.led_cdev  = {
			.name	         = "rx3000:blue",
			.default_trigger = "rx3000-radio",
			.flags		 = LED_SUPPORTS_HWTIMER,
		},
		.hw_num = 2,
	},
};

void rx3000_leds_release(struct device *dev)
{
	return;
}
  
static struct asic3_leds_machinfo rx3000_leds_machinfo = {
	.num_leds = ARRAY_SIZE(rx3000_leds),
	.leds = rx3000_leds,
	.asic3_pdev = &s3c_device_asic3,
};

static struct platform_device rx3000_leds_pdev = {
	.name = "asic3-leds",
	.dev = {
		.platform_data = &rx3000_leds_machinfo,
		.release = rx3000_leds_release,
	},
};

static int __init rx3000_led_init(void)
{
	int ret;

        printk(KERN_INFO "iPAQ RX3000 Leds Driver\n");
	led_trigger_register_shared("rx3000-radio", &rx3000_radio_trig);
	
	ret = asic3_leds_register();
	if (ret)
	    goto asic3_leds_failed;

	ret = platform_device_register(&rx3000_leds_pdev);
	if (ret)
	    goto platform_device_failed;
	
	goto success;

platform_device_failed:
	asic3_leds_unregister();
asic3_leds_failed:
	led_trigger_unregister_shared(rx3000_radio_trig);
success:
	return ret;
}

static void __exit rx3000_led_exit(void)
{
	led_trigger_unregister_shared(rx3000_radio_trig);
	platform_device_unregister(&rx3000_leds_pdev);
	asic3_leds_unregister();
}

module_init(rx3000_led_init);
module_exit(rx3000_led_exit);

MODULE_AUTHOR("Roman Moravcik <roman.moravcik@gmail.com>");
MODULE_DESCRIPTION("LED driver for HP iPAQ RX3000");
MODULE_LICENSE("GPL");
