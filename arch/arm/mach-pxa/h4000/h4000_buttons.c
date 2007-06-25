/*
 * Buttons driver for iPAQ h4150/h4350.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 * Copyright (C) 2005 Pawel Kolodziejski
 * Copyright (C) 2003 Joshua Wise
 *
 */

#include <linux/input.h>
#include <linux/input_pda.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
#include <linux/gpio_keys.h>
#include <linux/soc/asic3_base.h>
#include <asm/mach-types.h>
#include <asm/hardware/asic3_keys.h>
#include <asm/arch-pxa/h4000.h>
#include <asm/arch/h4000-gpio.h>
#include <asm/arch/h4000-asic.h>

extern struct platform_device h4000_asic3;

static struct gpio_keys_button pxa_buttons[] = {
	{ KEY_POWER,	GPIO_NR_H4000_POWER_BUTTON_N,	1, "Power button"	},
	{ KEY_ENTER,	GPIO_NR_H4000_ACTION_BUTTON_N,	1, "Action button"	},
	{ KEY_UP,	GPIO_NR_H4000_UP_BUTTON_N,	1, "Up button"		},
	{ KEY_DOWN,	GPIO_NR_H4000_DOWN_BUTTON_N,	1, "Down button"	},
	{ KEY_LEFT,	GPIO_NR_H4000_LEFT_BUTTON_N,	1, "Left button"	},
	{ KEY_RIGHT,	GPIO_NR_H4000_RIGHT_BUTTON_N,	1, "Right button"	},
};

static struct asic3_keys_button asic3_buttons[] = {
        { _KEY_RECORD,    H4000_RECORD_BTN_IRQ,		1, "Record button" },
        { _KEY_HOMEPAGE,  H4000_TASK_BTN_IRQ,		1, "Home button" },
        { _KEY_MAIL,      H4000_MAIL_BTN_IRQ,		1, "Mail button" },
        { _KEY_CONTACTS,  H4000_CONTACTS_BTN_IRQ, 	1, "Contacts button" },
        { _KEY_CALENDAR,  H4000_CALENDAR_BTN_IRQ,	1, "Calendar button" },
};

static struct gpio_keys_platform_data gpio_keys_data = {
        .buttons = pxa_buttons,
        .nbuttons = ARRAY_SIZE(pxa_buttons),
};

static struct asic3_keys_platform_data asic3_keys_data = {
        .buttons = asic3_buttons,
        .nbuttons = ARRAY_SIZE(asic3_buttons),
        .asic3_dev = &h4000_asic3.dev,
};

static struct platform_device h4000_keys_gpio = {
        .name = "gpio-keys",
        .dev = { .platform_data = &gpio_keys_data, }
};

static struct platform_device h4000_keys_asic3 = {
        .name = "asic3-keys",
        .dev = { .platform_data = &asic3_keys_data, }
};

static int __init h4000_buttons_probe(struct platform_device *dev)
{
	platform_device_register(&h4000_keys_gpio);
	platform_device_register(&h4000_keys_asic3);
	return 0;
}

static struct platform_driver h4000_buttons_driver = {
	.driver		= {
	    .name       = "h4000-buttons",
	},
	.probe          = h4000_buttons_probe,
};

static int __init h4000_buttons_init(void)
{
	if (!h4000_machine_is_h4000())
		return -ENODEV;

	return platform_driver_register(&h4000_buttons_driver);
}

static void __exit h4000_buttons_exit(void)
{
	platform_driver_unregister(&h4000_buttons_driver);
}

module_init(h4000_buttons_init);
module_exit(h4000_buttons_exit);

MODULE_AUTHOR ("Joshua Wise, Pawel Kolodziejski, Paul Sokolosvky");
MODULE_DESCRIPTION ("Buttons support for iPAQ h4000");
MODULE_LICENSE ("GPL");
