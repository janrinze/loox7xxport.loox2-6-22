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
#include <linux/soc/asic3_base.h>

#include <asm/hardware/ipaq-asic3.h>
#include <asm/mach-types.h>
#include <asm/hardware/asic3_leds.h>
#include <asm/arch/htcuniversal-asic.h>

DEFINE_LED_TRIGGER_SHARED_GLOBAL(htcuniversal_radio_trig);
EXPORT_LED_TRIGGER_SHARED(htcuniversal_radio_trig);

static struct asic3_led htcuniversal_leds[] = {
	{
		.led_cdev  = {
			.name	        = "htcuniversal:red",
			.default_trigger = "htcuniversal-charging",
		},
		.hw_num = 2,

	},
	{
		.led_cdev  = {
			.name	         = "htcuniversal:green",
			.default_trigger = "htcuniversal-chargefull",
		},
		.hw_num = 1,
	},
	{
		.led_cdev  = {
			.name	         = "htcuniversal:wifi-bt",
			.default_trigger = "htcuniversal-radio",
		},
		.hw_num = 0,
	},
	{
		.led_cdev  = {
			.name	         = "htcuniversal:phonebuttons",
			.default_trigger = "htcuniversal-phonebuttons",
		},
		.hw_num = -1,
		.gpio_num = ('D'-'A')*16+GPIOD_BL_KEYP_PWR_ON,
	},
	{
		.led_cdev  = {
			.name	         = "htcuniversal:vibra",
			.default_trigger = "htcuniversal-vibra",
		},
		.hw_num = -1,
		.gpio_num = ('D'-'A')*16+GPIOD_VIBRA_PWR_ON,
	},
	{
		.led_cdev  = {
			.name	         = "htcuniversal:flashlight1",
			.default_trigger = "htcuniversal-flashlight1",
		},
		.hw_num = -1,
		.gpio_num = ('A'-'A')*16+GPIOA_FLASHLIGHT,
	},
	{
		.led_cdev  = {
			.name	         = "htcuniversal:kbdbacklight",
			.default_trigger = "htcuniversal-kbdbacklight",
		},
		.hw_num = -1,
		.gpio_num = ('D'-'A')*16+GPIOD_BL_KEYB_PWR_ON,
	},
};

void htcuniversal_leds_release(struct device *dev)
{
	return;
}

static
struct asic3_leds_machinfo htcuniversal_leds_machinfo = {
	.num_leds = ARRAY_SIZE(htcuniversal_leds),
	.leds = htcuniversal_leds,
	.asic3_pdev = &htcuniversal_asic3,
};

static
struct platform_device htcuniversal_leds_pdev = {
	.name = "asic3-leds",
	.dev = {
		.platform_data = &htcuniversal_leds_machinfo,
		.release = htcuniversal_leds_release,
	},
};

static
int __init htcuniversal_leds_init(void)
{
	int ret;
	printk("htcuniversal LEDs Driver\n");
	led_trigger_register_shared("htcuniversal-radio", &htcuniversal_radio_trig);
	
	ret = asic3_leds_register();
	if (ret) goto asic3_leds_failed;

	ret = platform_device_register(&htcuniversal_leds_pdev);
	if (ret) goto platform_device_failed;
	
	goto success;

platform_device_failed:
	asic3_leds_unregister();
asic3_leds_failed:
	led_trigger_unregister_shared(htcuniversal_radio_trig);
	printk("htcuniversal LEDs Driver failed to init");
success:
	return ret;
}

static
void __exit htcuniversal_leds_exit(void)
{
	led_trigger_unregister_shared(htcuniversal_radio_trig);
	platform_device_unregister(&htcuniversal_leds_pdev);
	asic3_leds_unregister();
	return;
}

module_init(htcuniversal_leds_init);
module_exit(htcuniversal_leds_exit);

MODULE_AUTHOR("Anton Vorontsov <cbou@mail.ru>");
MODULE_DESCRIPTION("htcuniversal LEDs driver");
MODULE_LICENSE("GPL");
