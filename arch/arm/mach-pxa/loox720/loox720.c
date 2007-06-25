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

#include <asm/hardware/ipaq-asic3.h>
#include <linux/soc/asic3_base.h>

#include "../generic.h"

/* Initialization code */

static void __init loox720_map_io(void)
{
	pxa_map_io();
}

static void __init loox720_init_irq(void)
{
	/* int irq; */

	pxa_init_irq();
}

/* ASIC3*/

static struct asic3_platform_data loox720_asic3_platform_data = {
        .gpio_a = {
        //      .mask           = 0xffff,
                .dir            = 0xffff, // Unknown, set as outputs so far
                .init           = 0x0000,
        //      .trigger_type   = 0x0000,
        //      .edge_trigger   = 0x0000,
        //      .leveltri       = 0x0000,
        //      .sleep_mask     = 0xffff,
                .sleep_out      = 0x0000,
                .batt_fault_out = 0x0000,
        //      .int_status     = 0x0000,
                .alt_function   = 0xffff,
                .sleep_conf     = 0x000c,
        },
        .gpio_b = {
        //      .mask           = 0xffff,
                .dir            = 0xffff, // Unknown, set as outputs so far
                .init           = 0x0000,
        //      .trigger_type   = 0x0000,
        //      .edge_trigger   = 0x0000,
        //      .leveltri       = 0x0000,
        //      .sleep_mask     = 0xffff,
                .sleep_out      = 0x0000,
                .batt_fault_out = 0x0000,
        //      .int_status     = 0x0000,
                .alt_function   = 0xffff,
                .sleep_conf     = 0x000c,
        },
        .gpio_c = {
        //      .mask           = 0xffff,
                .dir            = 0x6067,
        // GPIOC_SD_CS_N | GPIOC_CIOW_N | GPIOC_CIOR_N  | GPIOC_PWAIT_N | GPIOC_PIOS16_N,
                .init           = 0x0000,
        //      .trigger_type   = 0x0000,
        //      .edge_trigger   = 0x0000,
        //      .leveltri       = 0x0000,
        //      .sleep_mask     = 0xffff,
                .sleep_out      = 0x0000,
                .batt_fault_out = 0x0000,
        //      .int_status     = 0x0000,
                .alt_function   = 0xfff7, // GPIOC_LED_RED | GPIOC_LED_GREEN | GPIOC_LED_BLUE,
                .sleep_conf     = 0x000c,
        },
        .gpio_d = {
        //      .mask           = 0xffff,
                .dir            = 0x0000, // Only inputs
                .init           = 0x0000,
        //      .trigger_type   = 0x67ff,
        //      .edge_trigger   = 0x0000,
        //      .leveltri       = 0x0000,
        //      .sleep_mask     = 0x9800,
                .sleep_out      = 0x0000,
                .batt_fault_out = 0x0000,
        //      .int_status     = 0x0000,
                .alt_function   = 0x9800,
                .sleep_conf     = 0x000c,
        },
        .bus_shift = 1,
	.irq_base = LOOX720_ASIC3_IRQ_BASE,
};


static struct resource asic3_resources[] = {
        [0] = {
                .start  = LOOX720_ASIC3_PHYS,
                .end    = LOOX720_ASIC3_PHYS + IPAQ_ASIC3_MAP_SIZE,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = LOOX720_IRQ(ASIC3_EXT_INT),
                .end    = LOOX720_IRQ(ASIC3_EXT_INT),
                .flags  = IORESOURCE_IRQ,
        },
};

struct platform_device loox720_asic3 = {
        .name           = "asic3",
        .id             = 0,
        .num_resources  = ARRAY_SIZE(asic3_resources),
        .resource       = asic3_resources,
        .dev = {
                .platform_data = &loox720_asic3_platform_data,
        },
};
EXPORT_SYMBOL(loox720_asic3);


/* PXA2xx Keys */

static struct gpio_keys_button loox720_button_table[] = {
	{ KEY_SUSPEND, GPIO_NR_LOOX720_KEY_ON, 1 },
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

static struct platform_device loox720_ts                = {
        .name = "loox720-ts",
};

static struct platform_device *devices[] __initdata = {
	&loox720_asic3,
	&loox720_pxa_keys,
	&pxa_spi_nssp,
	&loox720_ts,
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

	platform_add_devices( devices, ARRAY_SIZE(devices) );
}


MACHINE_START(LOOX720, "FSC Loox 720")
	/* Giuseppe Zompatori, <giuseppe_zompatori@yahoo.it> */
//	BOOT_MEM(0xaa000000, 0x40000000, io_p2v(0x40000000))
//	.phys_ram = 0xaa000000,
	.phys_io = 0x40000000,
	.io_pg_offst = (io_p2v(0x40000000) >> 18) & 0xfffc,
	.boot_params	= 0xaa000100,
	.map_io		= loox720_map_io,
	.init_irq	= loox720_init_irq,
	.timer = &pxa_timer,
	.init_machine = loox720_init,
MACHINE_END
