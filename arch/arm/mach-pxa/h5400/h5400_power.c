/*
 * Battery driver for iPAQ h5400
 *
 * Copyright 2007 Anton Vorontsov <cbou@mail.ru>
 * Copyright 2005 Phil Blundell
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * History:
 * 2007/02/23  Anton Vorontsov  Fix interrupts for the h5550, use
 *                              ds2760_battery driver, clean up.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pda_power.h>
#include <asm/mach-types.h>
#include <asm/hardware.h>
#include <asm/arch/pxa-regs.h>
#include <asm/arch/h5400-asic.h>
#include <asm/arch/h5400-gpio.h>
#include <asm/hardware/samcop_base.h>

static int h5000_is_ac_online(void)
{
	return !!(samcop_get_gpio_a(&h5400_samcop.dev) &
	         SAMCOP_GPIO_GPA_ADP_IN_STATUS);
}

static int h5000_is_usb_online(void)
{
	return !!(samcop_get_gpio_a(&h5400_samcop.dev) &
	         SAMCOP_GPIO_GPA_USB_DETECT);
}

static void h5000_set_charge(int flags)
{
	SET_H5400_GPIO(CHG_EN, !!flags);
	SET_H5400_GPIO(EXT_CHG_RATE, !!(flags & PDA_POWER_CHARGE_AC));
	SET_H5400_GPIO(USB_CHG_RATE, !!(flags & PDA_POWER_CHARGE_USB));
	return;
}

static char *h5000_supplicants[] = {
	"ds2760-battery.0"
};

static struct pda_power_pdata h5000_power_pdata = {
	.is_ac_online = h5000_is_ac_online,
	.is_usb_online = h5000_is_usb_online,
	.set_charge = h5000_set_charge,
	.supplied_to = h5000_supplicants,
	.num_supplicants = ARRAY_SIZE(h5000_supplicants),
};

static struct resource h5000_power_resourses[] = {
	[0] = {
		.name = "ac",
		.flags = IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHEDGE |
		         IORESOURCE_IRQ_LOWEDGE,
	},
	[1] = {
		.name = "usb",
		.flags = IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHEDGE |
		         IORESOURCE_IRQ_LOWEDGE,
	},
};

static void h5000_null_release(struct device *dev)
{
	/* current platform_device api is weird */
	return;
}

static struct platform_device h5000_power_pdev = {
	.name = "pda-power",
	.id = -1,
	.resource = h5000_power_resourses,
	.num_resources = ARRAY_SIZE(h5000_power_resourses),
	.dev = {
		.platform_data = &h5000_power_pdata,
		.release = h5000_null_release,
	},
};

static int h5000_power_init(void)
{
	int ret = 0, irq_base;
	unsigned int ac_irq, usb_irq;

	if (!machine_is_h5400())
		return -ENODEV;

	irq_base = samcop_irq_base(&h5400_samcop.dev);

	if (((read_cpuid(CPUID_ID) >> 4) & 0xfff) == 0x2d0) {
		/* h51xx | h55xx */
		ac_irq = irq_base + _IRQ_SAMCOP_WAKEUP1;
		usb_irq = irq_base + _IRQ_SAMCOP_WAKEUP2;
	}
	else {
		/* h54xx */
		ac_irq = irq_base + _IRQ_SAMCOP_WAKEUP2;
		usb_irq = irq_base + _IRQ_SAMCOP_WAKEUP3;
	}

	h5000_power_pdev.resource[0].start = ac_irq;
	h5000_power_pdev.resource[0].end = ac_irq;
	h5000_power_pdev.resource[1].start = usb_irq;
	h5000_power_pdev.resource[1].end = usb_irq;

	ret = platform_device_register(&h5000_power_pdev);
	if (ret)
		printk("h5000_power: registration failed\n");

	return ret;
}

static void h5000_power_exit(void)
{
	platform_device_unregister(&h5000_power_pdev);
	return;
}

module_init(h5000_power_init);
module_exit(h5000_power_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anton Vorontsov <cbou@mail.ru>");
