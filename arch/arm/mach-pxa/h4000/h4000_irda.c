/*
 * h4000_udc.c:
 * h4000 specific code for the pxa255 IrDA controller.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dpm.h>
#include <asm/arch/pxa-regs.h>
#include <asm/arch/irda.h>
#include <linux/soc/asic3_base.h>
#include <asm/arch/h4000-gpio.h>
#include <asm/arch/h4000-asic.h>

extern struct platform_device h4000_asic3;


/* for pxaficp_ir */
static void h4000_irda_transceiver_mode(struct device *dev, int mode)
{
	unsigned long flags;

	local_irq_save(flags);

	if (mode & IR_OFF) {
		DPM_DEBUG("h4000_irda: Turning off port\n");
		asic3_set_gpio_out_d(&h4000_asic3.dev, GPIOD_IR_ON_N, GPIOD_IR_ON_N);
	} else {
		DPM_DEBUG("h4000_irda: Turning on port\n");
		asic3_set_gpio_out_d(&h4000_asic3.dev, GPIOD_IR_ON_N, 0);
	}

	local_irq_restore(flags);
}

static struct pxaficp_platform_data h4000_ficp_platform_data = {
	.transceiver_cap  = IR_SIRMODE | IR_OFF,
	.transceiver_mode = h4000_irda_transceiver_mode,
};



static int h4000_irda_probe(struct platform_device * dev)
{
	pxa_set_ficp_info(&h4000_ficp_platform_data);
	return 0;
}

static struct platform_driver h4000_irda_driver = {
	.driver	  = {
		.name     = "h4000-irda",
	},
	.probe    = h4000_irda_probe,
};

static int __init h4000_irda_init(void)
{
	return platform_driver_register(&h4000_irda_driver);
}

module_init(h4000_irda_init);
MODULE_LICENSE("GPL");
