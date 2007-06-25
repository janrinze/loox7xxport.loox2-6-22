/*
 * HTC Magician LED driver
 *
 * Copyright (c) 2006 Philipp Zabel <philipp.zabel@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>

#include <asm/io.h>
#include <asm/gpio.h>

#include <asm/mach-types.h>
#include <asm/arch/magician.h>

struct pasic3_data;

struct pasic3_led {
	struct led_classdev led;
	int bit;
	int bit2;
	int mask;
	struct pasic3_data *pasic3_data;
};

/*
 * mask for register 0x20,0x21,0x22
 */
#define LED_NUM0           0x04	/* mask red */
#define LED_NUM1           0x08	/* mask green */
#define LED_NUM2           0x40	/* mask blue */

/*
 * bits in register 0x06
 */
#define LED_RED    0x01
#define LED_GREEN  0x02
#define LED_BLUE   0x04
#define LED_RED2   0x08		/* higher voltage --> brighter */
#define LED_GREEN2 0x10		/* brighter */
#define LED_BLUE2  0x20		/* same brightness as 0x04 */

/* wince uses 0x18 for the "charging" orange */

/*
 * map should be drvdata, bus_shift and num_leds platform_data
 */
struct pasic3_data {
	void *map;
	int   bus_shift; /* # of shifts to calculate register offsets */
	int   num_leds;  /* determines # duty/cycle registers and pos. of voltage register */
};

/*
 * this should be platform_data
 */
struct pasic3_data pasic3_data = {
	.bus_shift = 2,
	.num_leds  = 3,
};

/*
 * write to a secondary register on the PASIC3
 */
static void pasic3_write_register(struct pasic3_data *pasic3_data, u32 reg, u8 val)
{
	int bus_shift = pasic3_data->bus_shift;
	void *addr = pasic3_data->map + (5 << bus_shift);
	void *data = pasic3_data->map + (6 << bus_shift);

	__raw_writew((__raw_readw(addr) & 0xff80) | (reg & 0x7f), addr);
	__raw_writew((__raw_readw(addr) & 0xff7f), addr); /* write mode */

	__raw_writeb((__raw_readb(data) & 0xff00) | (val & 0xff), data);
}

/*
 * read from a secondary register on the PASIC3
 */
static u8 pasic3_read_register(struct pasic3_data *pasic3_data, u32 reg)
{
	int bus_shift = pasic3_data->bus_shift;
	void *addr = pasic3_data->map + (5 << bus_shift);
	void *data = pasic3_data->map + (6 << bus_shift);

	__raw_writew((__raw_readw(addr) | 0x80), addr); /* read mode */
	__raw_writew((__raw_readw(addr) & 0xff80) | (reg & 0x7f), addr);

	return __raw_readb(data);
}

/*
 * The purpose of the following two functions is unknown.
 */
static void pasic3_enable_led_mask(struct pasic3_data *pasic3_data, int mask)
{
	int r20, r21, r22;

	r20 = pasic3_read_register(pasic3_data, 0x20);
	r21 = pasic3_read_register(pasic3_data, 0x21);
	r22 = pasic3_read_register(pasic3_data, 0x22);
	r20 &= ~mask;
	pasic3_write_register(pasic3_data, 0x20, r20);
	pasic3_write_register(pasic3_data, 0x21, r21);
	pasic3_write_register(pasic3_data, 0x22, r22);
}

/*
 * The purpose of the following function is unknown.
 */
static void pasic3_disable_led_mask(struct pasic3_data *pasic3_data, int mask)
{
	int r20, r21, r22;

	r20 = pasic3_read_register(pasic3_data, 0x20);
	r21 = pasic3_read_register(pasic3_data, 0x21);
	r22 = pasic3_read_register(pasic3_data, 0x22);
	r20 |= mask;
	r21 |= mask;	/* what are 0x21 and 0x22 good for? */
	r22 &= ~mask;	/* the blueangel driver doesn't seem to need them */
	pasic3_write_register(pasic3_data, 0x20, r20);
	pasic3_write_register(pasic3_data, 0x21, r21);
	pasic3_write_register(pasic3_data, 0x22, r22);
}

/*
 * led_classdev callback to set the brightness of a pasic3_led
 */
static void pasic3_led_set(struct led_classdev *led_cdev,
					enum led_brightness value)
{
	struct pasic3_led *led = container_of(led_cdev, struct pasic3_led, led);
	struct pasic3_data *pasic3_data = led->pasic3_data;
	int led_reg = pasic3_data->num_leds * 2; /* after on and off time registers */
	int state;

	state = pasic3_read_register(pasic3_data, led_reg);

	if (value) {
		pasic3_enable_led_mask(pasic3_data, led->mask);
		if ((value > LED_HALF) && (led->bit2))
			state = (state & ~led->bit) | led->bit2;
		else
			state = (state & ~led->bit2) | led->bit;
	} else {
		pasic3_disable_led_mask(pasic3_data, led->mask);
		state &= ~(led->bit | led->bit2);
	}

	pasic3_write_register(pasic3_data, led_reg, state);
}

static void magician_led_keys_set(struct led_classdev *led_cdev,
					enum led_brightness value)
{
	gpio_set_value(GPIO103_MAGICIAN_LED_KP, value);
}

static void magician_led_vibra_set(struct led_classdev *led_cdev,
					enum led_brightness value)
{
	gpio_set_value(GPIO22_MAGICIAN_VIBRA_EN, value);
}

static struct pasic3_led magician_pasic3_leds[] = {
	{
		.led = {
			.name            = "magician:red",
			.default_trigger = "ds2760-battery.0-charging", /* and alarm */
			.brightness_set  = pasic3_led_set,
		},
		.bit  = LED_RED,
		.bit2 = LED_RED2,
		.mask = LED_NUM0,
		.pasic3_data = &pasic3_data,
	},
	{
		.led = {
			.name            = "magician:green",
			.default_trigger = "ds2760-battery.0-charging-or-full", /* and GSM radio */
			.brightness_set  = pasic3_led_set,
		},
		.bit  = LED_GREEN,
		.bit2 = LED_GREEN2,
		.mask = LED_NUM1,
		.pasic3_data = &pasic3_data,
	},
	{
		.led = {
			.name            = "magician:blue",
			.default_trigger = "magician-bluetooth",
			.brightness_set  = pasic3_led_set,
		},
		.bit  = LED_BLUE,
		.bit2 = LED_BLUE2,
		.mask = LED_NUM2,
		.pasic3_data = &pasic3_data,
	},
};

static struct led_classdev magician_keys_led = {
	.name			= "magician:keys",
	.default_trigger	= "backlight",
	.brightness_set		= magician_led_keys_set,
};

static struct led_classdev magician_vibra_led = {
	.name			= "magician:vibra",
	.default_trigger	= "none",
	.brightness_set		= magician_led_vibra_set,
};

/*
 * red + green = amber/orange
 * red + blue = pink
 * blue + green = cyan
 * red + green + blue = pink
 */

#ifdef CONFIG_PM
static int magician_led_suspend(struct platform_device *dev, pm_message_t state)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(magician_pasic3_leds); i++)
		led_classdev_suspend(&magician_pasic3_leds[i].led);

	led_classdev_suspend(&magician_keys_led);
	led_classdev_suspend(&magician_vibra_led);

	/* TODO: leave the leds blinking if bt/gsm is enabled */

	return 0;
}

static int magician_led_resume(struct platform_device *dev)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(magician_pasic3_leds); i++)
		led_classdev_resume(&magician_pasic3_leds[i].led);
	led_classdev_resume(&magician_keys_led);
	led_classdev_resume(&magician_vibra_led);

	return 0;
}
#else
#define magician_led_suspend NULL
#define magician_led_resume NULL
#endif

static int magician_led_probe(struct platform_device *pdev)
{
	int ret;
	int i;
	int state;

	for (i = 0; i < ARRAY_SIZE(magician_pasic3_leds); i++) {
		ret = led_classdev_register(&pdev->dev, &magician_pasic3_leds[i].led);
		if (ret < 0)
			goto pasic3_err;
	}

	ret = led_classdev_register(&pdev->dev, &magician_keys_led);
	if (ret < 0)
		goto keys_err;

	ret = led_classdev_register(&pdev->dev, &magician_vibra_led);
	if (ret < 0)
		goto vibra_err;


	magician_egpio_set(EGPIO_NR_MAGICIAN_LED_POWER, 1);

	pasic3_data.map = ioremap_nocache(0x08000000, 0x1000);

	pasic3_write_register(&pasic3_data, 0x00,0); /* blue on time */
	pasic3_write_register(&pasic3_data, 0x01,0); /* blue off time */
	pasic3_write_register(&pasic3_data, 0x02,0); /* green on time, 0x10 = ~1s ((ms*16+500)/1000) */
	pasic3_write_register(&pasic3_data, 0x03,0); /* green off time, */
	pasic3_write_register(&pasic3_data, 0x04,0); /* blue duty time, (cycle = duty+off) */
	pasic3_write_register(&pasic3_data, 0x05,0); /* blue off time, */

	pasic3_disable_led_mask(&pasic3_data, LED_NUM0 | LED_NUM1 | LED_NUM2);

	state = pasic3_read_register(&pasic3_data, 0x06);
	pasic3_write_register(&pasic3_data, 0x06, state & ~0x3f);

	return ret;

vibra_err:
	led_classdev_unregister(&magician_keys_led);
keys_err:
pasic3_err:
	for (i = i-1; i >= 0; i--)
		led_classdev_unregister(&magician_pasic3_leds[i].led);
	return ret;
}

static int magician_led_remove(struct platform_device *pdev)
{
	int i;

	iounmap(pasic3_data.map);

	magician_egpio_set(EGPIO_NR_MAGICIAN_LED_POWER, 0);

	led_classdev_unregister(&magician_vibra_led);
	led_classdev_unregister(&magician_keys_led);

	for (i = 0; i < ARRAY_SIZE(magician_pasic3_leds); i++)
		led_classdev_unregister(&magician_pasic3_leds[i].led);

	return 0;
}

static struct platform_driver magician_led_driver = {
	.probe		= magician_led_probe,
	.remove		= magician_led_remove,
	.suspend	= magician_led_suspend,
	.resume		= magician_led_resume,
	.driver		= {
		.name		= "magician-led",
	},
};

static int __init magician_led_init (void)
{
	return platform_driver_register(&magician_led_driver);
}

static void __exit magician_led_exit (void)
{
 	platform_driver_unregister(&magician_led_driver);
}

module_init (magician_led_init);
module_exit (magician_led_exit);

MODULE_AUTHOR("Philipp Zabel <philipp.zabel@gmail.com>");
MODULE_DESCRIPTION("HTC Magician LED driver");
MODULE_LICENSE("GPL");
