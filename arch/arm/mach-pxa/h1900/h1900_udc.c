/*
 *  HP iPAQ h1910/h1915 UDC glue driver
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  Copyright (C) 2005-2007 Pawel Kolodziejski
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/major.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/soc/asic3_base.h>

#include <asm/setup.h>
#include <asm/memory.h>
#include <asm/hardware.h>
#include <asm/irq.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <asm/arch/pxa-regs.h>
#include <asm/arch/h1900-asic.h>
#include <asm/arch/h1900-gpio.h>
#include <asm/arch/udc.h>

extern struct platform_device h1900_asic3;
#define asic3 &h1900_asic3.dev

static int h1900_udc_is_connected(void)
{
	return (!(GPLR(GPIO_NR_H1900_USB_DETECT2_N) & GPIO_bit(GPIO_NR_H1900_USB_DETECT2_N)));
}

static void h1900_udc_command(int cmd)
{
	switch (cmd) {
		case PXA2XX_UDC_CMD_DISCONNECT:
			asic3_set_gpio_out_d(asic3, GPIO4_H1900_USB, 0);
			break;
		case PXA2XX_UDC_CMD_CONNECT:
			asic3_set_gpio_out_d(asic3, GPIO4_H1900_USB, GPIO4_H1900_USB);
			break;
		default:
			break;
	}
}

static struct pxa2xx_udc_mach_info h1900_udc_mach_info __initdata = {
	.udc_is_connected = h1900_udc_is_connected,
	.udc_command      = h1900_udc_command,
};

static int h1900_udc_probe(struct platform_device *pdev)
{
	pxa_set_udc_info(&h1900_udc_mach_info);

	return 0;
}

static struct platform_driver h1900_udc_driver = {
	.driver   = {
		.name     = "h1900-udc",
	},
	.probe    = h1900_udc_probe,
};

static int __init h1900_udc_init(void)
{
	return platform_driver_register(&h1900_udc_driver);
}

#ifdef MODULE
module_init(h1900_udc_init);
#else   /* start early for dependencies */
fs_initcall(h1900_udc_init);
#endif

MODULE_AUTHOR("Pawel Kolodziejski");
MODULE_DESCRIPTION("HP iPAQ h1910/h1915 UDC glue driver");
MODULE_LICENSE("GPL");
