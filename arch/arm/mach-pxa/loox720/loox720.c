/*
 * 
 * Hardware definitions for HP iPAQ Handheld Computers
 *
 * Copyright 2004 Hewlett-Packard Company.
 *
 * Use consistent with the GNU GPL is permitted,
 * provided that this copyright notice is
 * preserved in its entirety in all copies and derived works.
 *
 * COMPAQ COMPUTER CORPORATION MAKES NO WARRANTIES, EXPRESSED OR IMPLIED,
 * AS TO THE USEFULNESS OR CORRECTNESS OF THIS CODE OR ITS
 * FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 * History:
 *
 * 2004-11-2004	Michael Opdenacker	Preliminary version
 * 2004-12-16   Todd Blumer
 * 2004-12-22   Michael Opdenacker	Added USB management
 * 2005-01-30   Michael Opdenacker	Improved Asic3 settings and initialization
 */


#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#include <asm/mach-types.h>
#include <asm/hardware.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <asm/arch/loox720-gpio.h>
#include <asm/arch/loox720-asic.h>
#include <asm/arch/pxa-regs.h>
#include <linux/gpio_keys.h>
#include <asm/arch/pxa2xx_spi.h>
#include <asm/arch/udc.h>
#include <asm/arch/audio.h>

#include <linux/corgi_bl.h>
#include <asm/arch/pxa2xx_udc_gpio.h>

#include "../generic.h"
#include "loox720_core.h"
#include <asm/arch/loox720-cpld.h>

/* PXA2xx Keys */

static struct gpio_keys_button loox720_button_table[] = {
	{ KEY_POWER, GPIO_NR_LOOX720_KEY_ON, 1 },
};

static struct gpio_keys_platform_data loox720_pxa_keys_data = {
	.buttons = loox720_button_table,
	.nbuttons = ARRAY_SIZE(loox720_button_table),
};

static struct platform_device loox720_pxa_keys = {
	.name = "gpio-keys",
	.dev = {
		.platform_data = &loox720_pxa_keys_data,
	},
};

// SPI START

static struct resource pxa_spi_nssp_resources[] = {
	[0] = {
		.start	= __PREG(SSCR0_P1), /* Start address of NSSP */
		.end	= __PREG(SSCR0_P1) + 0x2c, /* Range of registers */
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_SSP, /* NSSP IRQ */
		.end	= IRQ_SSP,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct pxa2xx_spi_master pxa_nssp_master_info = {
	.ssp_type = PXA27x_SSP, /* Type of SSP */
	.clock_enable = CKEN23_SSP1, /* NSSP Peripheral clock */
	.num_chipselect = 1, /* Matches the number of chips attached to NSSP */
	.enable_dma = 0, /* Enables NSSP DMA */
};

static struct platform_device pxa_spi_nssp = {
	.name = "pxa2xx-spi", /* MUST BE THIS VALUE, so device match driver */
	.id = 1, /* Bus number, MUST MATCH SSP number 1..n */
	.resource = pxa_spi_nssp_resources,
	.num_resources = ARRAY_SIZE(pxa_spi_nssp_resources),
	.dev = {
		.platform_data = &pxa_nssp_master_info, /* Passed to driver */
	},
};

// SPI END

static struct platform_device loox720_ts = {
	.name = "loox720-ts",
};

#define LOOX720_MAX_INTENSITY 0xc8
#define LOOX720_DEFAULT_INTENSITY (LOOX720_MAX_INTENSITY / 4)

static void loox720_set_bl_intensity(int intensity)
{
	if (intensity < 7) intensity = 0;

	PWM_CTRL0 = 1;
	PWM_PWDUTY0 = intensity;
	PWM_PERVAL0 = LOOX720_MAX_INTENSITY;

	if (intensity > 0) {
		loox720_egpio_set_bit(LOOX720_CPLD_BACKLIGHT_BIT, 1);
		pxa_set_cken(CKEN0_PWM0, 1);
	} else {
		loox720_egpio_set_bit(LOOX720_CPLD_BACKLIGHT_BIT, 0);
                pxa_set_cken(CKEN0_PWM0, 0);
	}
}

static struct corgibl_machinfo loox720_bl_machinfo = {
        .default_intensity = LOOX720_DEFAULT_INTENSITY,
        .limit_mask = 0xff,
        .max_intensity = LOOX720_MAX_INTENSITY,
        .set_bl_intensity = loox720_set_bl_intensity,
};

struct platform_device loox720_bl = {
        .name = "corgi-bl",
        .dev = {
    		.platform_data = &loox720_bl_machinfo,
	},
};

static struct platform_device loox720_buttons = {
	.name = "loox720-buttons",
};

static struct platform_device loox720_battery = {
	.name = "loox720-battery",
};

static struct loox720_core_funcs core_funcs;

static struct platform_device loox720_core = {
	.name = "loox720-core",
	.id		= -1,
	.dev = {
		.platform_data = &core_funcs,
	},
};

/*struct pxa2xx_udc_gpio_info loox720_udc_info = {
	.detect_gpio = {&loox720_asic3.dev, ASIC3_GPIOD_IRQ_BASE + GPIOD_USBC_DETECT_N},
	.detect_gpio_negative = 1,
	.power_ctrl = {
		.power_gpio = {&pxagpio_device.dev, GPIO_NR_LOOX720_USB_PUEN},
	},

};*/

/*static void
udc_enable(int cmd) 
{
	switch (cmd)
	{
		case PXA2XX_UDC_CMD_DISCONNECT:
			printk (KERN_NOTICE "USB cmd disconnect\n");
			loox720_udc_disable(0x00020000);
			break;

		case PXA2XX_UDC_CMD_CONNECT:
			printk (KERN_NOTICE "USB cmd connect\n");
			SET_X30_GPIO(USB_PUEN, 1);
			break;
	}
}*/

static struct pxa2xx_udc_mach_info loox720_udc_info __initdata = {
//	.gpio_pullup = GPIO_NR_LOOX720_USB_PULLUP,

};

/*static struct platform_device loox720_udc = { 
	.name = "pxa2xx-udc-gpio",
	.dev = {
		.platform_data = &loox720_udc_info
	}
};*/

static struct platform_device *devices[] __initdata = {
	&loox720_core,
	&pxa_spi_nssp,
	&loox720_buttons,
	&loox720_ts,
	&loox720_pxa_keys,
	&loox720_bl,
	&loox720_battery,
//	&loox720_udc,
};

static void __init loox720_init( void )
{
#if 0	// keep for reference, from bootldr
	GPSR0 = 0x0935ede7;
	GPSR1 = 0xffdf40f7;
	GPSR2 = 0x0173c9f6;
	GPSR3 = 0x01f1e342;
	GPCR0 = ~0x0935ede7;
	GPCR1 = ~0xffdf40f7;
	GPCR2 = ~0x0173c9f6;
	GPCR3 = ~0x01f1e342;
	GPDR0 = 0xda7a841c;
	GPDR1 = 0x68efbf83;
	GPDR2 = 0xbfbff7db;
	GPDR3 = 0x007ffff5;
	GAFR0_L = 0x80115554;
	GAFR0_U = 0x591a8558;
	GAFR1_L = 0x600a9558;
	GAFR1_U = 0x0005a0aa;
	GAFR2_L = 0xa0000000;
	GAFR2_U = 0x00035402;
	GAFR3_L = 0x00010000;
	GAFR3_U = 0x00001404;
	MSC0 = 0x25e225e2;
	MSC1 = 0x12cc2364;
	MSC2 = 0x16dc7ffc;
#endif

	SET_LOOX720_GPIO( ASIC3_RESET_N, 0 );
	mdelay(50);
	SET_LOOX720_GPIO( ASIC3_RESET_N, 1 );
	mdelay(50);
	
	pxa_set_udc_info(&loox720_udc_info);

	platform_add_devices( devices, ARRAY_SIZE(devices) );
}


MACHINE_START(LOOX720, "FSC Loox 720")
	/* Giuseppe Zompatori, <giuseppe_zompatori@yahoo.it> */
//	BOOT_MEM(0xaa000000, 0x40000000, io_p2v(0x40000000))
//	.phys_ram = 0xaa000000,
	.phys_io = 0x40000000,
	.io_pg_offst = (io_p2v(0x40000000) >> 18) & 0xfffc,
	.boot_params	= 0xaa000100,
	.map_io		= pxa_map_io,
	.init_irq	= pxa_init_irq,
	.timer =	&pxa_timer,
	.init_machine = loox720_init,
MACHINE_END
