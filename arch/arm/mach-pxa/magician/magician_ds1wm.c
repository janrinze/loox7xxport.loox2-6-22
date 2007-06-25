/*
 * magician_ds1wm.c - HTC Magician DS1WM-in-AIC3 driver
 *
 * Copyright (C) 2006 Philipp Zabel <philipp.zabel@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/ds1wm.h>

#include <asm/gpio.h>

#include <asm/arch/magician.h>
#include <asm/arch/pxa-regs.h>
#include <asm/arch/clock.h>

unsigned int __iomem *iobase;

static int read_register(int reg)
{
	u16 *addr=((u16 *)(iobase+5));
	u8 *data=((u8 *)(iobase+6));

	*addr |= 0x80; /* read mode */
	*addr = (*addr & 0xff80) | (reg & 0x7f);

	barrier();
	return *data;
}

static void write_register(int reg, int val)
{
	u16 *addr=((u16 *)(iobase+5));
	u16 *data=((u16 *)(iobase+6));

	*addr = (*addr & 0xff80) | (reg & 0x7f);
	*addr &= 0xff7f; /* write mode */

	barrier();
	*data = (*data & 0xff00) | (val & 0xff);
}

static struct resource __devinit magician_ds1wm_resources[] = {
	[0] = {
		.start  = PXA_CS2_PHYS,
		.end	= PXA_CS2_PHYS + 0x10,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start  = gpio_to_irq(GPIO107_MAGICIAN_DS1WM_IRQ),
		.end    = gpio_to_irq(GPIO107_MAGICIAN_DS1WM_IRQ),
		.flags  = IORESOURCE_IRQ,
	}
};

static struct ds1wm_platform_data __devinit magician_ds1wm_platform_data = {
	.bus_shift = 2,
};

static struct platform_device *magician_ds1wm;
#if 0
 = {
	.name		= "ds1wm",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(magician_ds1wm_resources),
	.resource	= magician_ds1wm_resources,
	.dev =  {
		.platform_data = magician_ds1wm_platform_data,
		},
};
#endif

static void magician_ds1wm_enable(struct clk *clock)
{
	int c;

	/* I don't know how to enable the 4MHz OWM clock here */
	write_register(0x2a, 0x08); /* sets up the 4MHz clock? */

	c = read_register(0x28);
	write_register(0x28, c & 0x7f);

	printk ("magician_ds1wm: OWM_EN low (active) %02x\n", c & 0x7f);
}

static void magician_ds1wm_disable(struct clk *clock)
{
	int c;
	c = read_register(0x28);
	write_register(0x28, c | 0x80);

	printk ("magician_ds1wm: OWM_EN high (inactive) %02x\n", c | 0x80);

	/* I don't know how to disable the 4MHz OWM clock here */
}

static struct clk ds1wm_clk = {
        .name    = "ds1wm",
        .rate    = 4000000,
        .parent  = NULL,
	.enable  = magician_ds1wm_enable,
	.disable = magician_ds1wm_disable,
};

static int __devinit magician_ds1wm_init(void)
{
        int ret;

	printk("Magician DS1WM driver\n");
	iobase = ioremap_nocache(PXA_CS2_PHYS, 0x18); // map PASIC3 for EGPIO registers

	if (clk_register(&ds1wm_clk) < 0)
		printk(KERN_ERR "failed to register DS1WM clock\n");

	magician_ds1wm = platform_device_alloc("ds1wm", -1);
        if (!magician_ds1wm) {
		printk("magician_ds1wm: failed to allocate platform device\n");
		ret = -ENOMEM;
		goto exit_unmap;
	}

	set_irq_type(gpio_to_irq(GPIO107_MAGICIAN_DS1WM_IRQ), IRQ_TYPE_EDGE_RISING);

	magician_ds1wm->num_resources = ARRAY_SIZE(magician_ds1wm_resources);
	ret = platform_device_add_resources(magician_ds1wm, magician_ds1wm_resources, ARRAY_SIZE(magician_ds1wm_resources));
	ret = platform_device_add_data(magician_ds1wm, &magician_ds1wm_platform_data, sizeof(magician_ds1wm_platform_data));

        ret = platform_device_add(magician_ds1wm);

        if (ret) {
                platform_device_put(magician_ds1wm);
		printk("magician_ds1wm: failed to add platform device\n");
	}

        return ret;

exit_unmap:
	iounmap((void __iomem *)iobase);
	return ret;
}

static void __devexit magician_ds1wm_exit(void)
{
	platform_device_unregister(magician_ds1wm);
	clk_unregister(&ds1wm_clk);
	iounmap((void __iomem *)iobase);
}

MODULE_AUTHOR("Philipp Zabel <philipp.zabel@gmail.com>");
MODULE_DESCRIPTION("DS1WM driver");
MODULE_LICENSE("GPL");

module_init(magician_ds1wm_init);
module_exit(magician_ds1wm_exit);
