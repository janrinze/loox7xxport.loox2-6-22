#if 1
/*
 * Touchscreen driver for Axim X50/X51(v).
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 * Copyright (C) 2007 Pierre Gaufillet
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/ads7846.h>
#include <linux/touchscreen-adc.h>
#include <asm/irq.h>
#include <asm/mach-types.h>
#include <asm/hardware.h>
#include <asm/setup.h>
#include <asm/mach/irq.h>
#include <asm/mach/arch.h>
#include <asm/arch/bitfield.h>
#include <asm/arch/pxa-regs.h>
#include <asm/arch/loox720-gpio.h>

#include "../generic.h"

static struct ads7846_ssp_platform_data loox720_ts_ssp_params = {
    .port = 1,
    .pd_bits = 1,
    .freq = 50000,
};
static struct platform_device ads7846_ssp     = {
    .name = "ads7846-ssp",
    .id = -1,
    .dev = {
        .platform_data = &loox720_ts_ssp_params,
    }
};

static struct tsadc_platform_data loox720_ts_params = {
//    .pen_irq = IRQ_GPIO(GPIO_NR_LOOX720_TOUCHPANEL_IRQ_N),
    .pen_gpio = GPIO_NR_LOOX720_TOUCHPANEL_IRQ_N,
    .x_pin = "ads7846-ssp:x",
    .y_pin = "ads7846-ssp:y",
    .z1_pin = "ads7846-ssp:z1",
    .z2_pin = "ads7846-ssp:z2",
    .pressure_factor = 100000,
    .min_pressure = 2,
    .delayed_pressure=1,
    .max_jitter = 8,
};
static struct resource loox720_pen_irq = {
	.start = IRQ_GPIO(GPIO_NR_LOOX720_TOUCHPANEL_IRQ_N),
	.end = IRQ_GPIO(GPIO_NR_LOOX720_TOUCHPANEL_IRQ_N),
	.flags = IORESOURCE_IRQ,
};
static struct platform_device loox720_ts        = {
    .name = "ts-adc-debounce",
    .id = -1,
    .resource = &loox720_pen_irq,
    .num_resources = 1,
    .dev = {
        .platform_data = &loox720_ts_params,
    }
};

static int __devinit loox720_ts_probe(struct platform_device *dev)
{
    platform_device_register(&ads7846_ssp);
    platform_device_register(&loox720_ts);
    return 0;
}

static struct platform_driver loox720_ts_driver = {
    .driver        = {
        .name       = "loox720-ts",
    },
    .probe          = loox720_ts_probe,
};

static int __init loox720_ts_init(void)
{
    if (!machine_is_loox720())
        return -ENODEV;

    return platform_driver_register(&loox720_ts_driver);
}

static void __exit loox720_ts_exit(void)
{
    platform_driver_unregister(&loox720_ts_driver);
}


module_init(loox720_ts_init);
module_exit(loox720_ts_exit);

MODULE_AUTHOR ("Pierre Gaufillet");
MODULE_DESCRIPTION ("Touchscreen support for LOOX718/720");
MODULE_LICENSE ("GPL");
#else
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>

#include <asm/arch/pxa-regs.h>

#include <linux/spi/spi.h>
#include <linux/spi/ads7846.h>
#include <asm/arch/pxa2xx_spi.h>
#include <asm/arch/loox720-gpio.h>
#include <asm/arch/loox720-asic.h>


#include <asm/mach/map.h>
#include <asm/mach-types.h>
#include <asm/arch/hardware.h>

// Device-specific information
static struct ads7846_platform_data platform_info = {
    .model                  = 7846,
    .vref_delay_usecs       = 0,
    .x_plate_ohms           = 0,
    .y_plate_ohms           = 0,
    .x_min                  = 0,
    .x_max                  = 4095,
    .y_min                  = 0,
    .y_max                  = 4095,
    .pressure_min           = 0,
    .pressure_max           = 4095,
};

static void ads7846_cs_control(u32 command)
{
    //printk("CS Control Called ...\n");
    if (command == PXA2XX_CS_ASSERT) GPCR(24) = GPIO_bit(24);
    if (command == PXA2XX_CS_DEASSERT) GPSR(24) = GPIO_bit(24);
}

static struct pxa2xx_spi_chip chip_info = {
    // The "pxa2xx_spi_chip.tx_threshold" and "pxa2xx_spi_chip.rx_threshold" fields are
    // used to configure the SSP hardware fifo.  These fields are critical to the
    // performance of pxa2xx_spi driver and misconfiguration will result in rx
    // fifo overruns (especially in PIO mode transfers).
    // Good default values are 12 (tx) and 4 (rx).
    .tx_threshold = 12,
    .rx_threshold = 4,

    // The "pxa2xx_spi_chip.dma_burst_size" field is used to configure PXA2xx DMA
    // engine and is related the "spi_device.bits_per_word" field.  Read and understand
    // the PXA2xx "Developer Manual" sections on the DMA controller and SSP Controllers
    // to determine the correct value. An SSP configured for byte-wide transfers would
    // use a value of 8.
    .dma_burst_size = 8,

    // The "pxa2xx_spi_chip.timeout_microsecs" fields is used to efficiently handle
    // trailing bytes in the SSP receiver fifo.  The correct value for this field is
    // dependent on the SPI bus speed ("spi_board_info.max_speed_hz") and the specific
    // slave device.  Please note the the PXA2xx SSP 1 does not support trailing byte
    // timeouts and must busy-wait any trailing bytes.
    .timeout = 0,

    // The "pxa2xx_spi_chip.enable_loopback" field is used to place the SSP porting
    // into internal loopback mode.  In this mode the SSP controller internally
    // connects the SSPTX pin the the SSPRX pin.  This is useful for initial setup
    // testing.
    .enable_loopback = 0,

    // The "pxa2xx_spi_chip.cs_control" field is used to point to a board specific
    // function for asserting/deasserting a slave device chip select.  If the field is
    // NULL, the pxa2xx_spi master controller driver assumes that the SSP port is
    // configured to use SSPFRM instead.
    .cs_control = ads7846_cs_control,
};

static struct spi_board_info loox720_board_info[] __initdata = {
    {
        .modalias       = "ads7846",
    //    .platform_data    = NULL,
        .platform_data  = &platform_info,
        .controller_data = &chip_info,
        .irq            = LOOX720_IRQ(TOUCHPANEL_IRQ_N),
        .max_speed_hz   = 100000, /* max sample rate at 3V */ //* 16,
        .bus_num        = 1,
        .chip_select    = 0,
    },
};

// Init and exit functions
static int __init loox720_ts_init(void)
{
    int retval = 0;

    //if(!machine_is_loox720()) return -ENODEV;

        /* now to set up GPIOs... */
        GPDR(GPIO23_SCLK) |=  GPIO_bit(GPIO23_SCLK);
        GPDR(GPIO24_SFRM) |=  GPIO_bit(GPIO24_SFRM);
        GPDR(GPIO25_STXD) |=  GPIO_bit(GPIO25_STXD);
        GPDR(GPIO26_SRXD) &= ~GPIO_bit(GPIO26_SRXD);
    GPDR(94) &= ~ GPIO_bit(94); /* PENIRQ as input */
        pxa_gpio_mode(GPIO23_SCLK_MD);
        pxa_gpio_mode(24|GPIO_OUT); /* Manual Chip Select used */
        pxa_gpio_mode(GPIO25_STXD_MD);
        pxa_gpio_mode(GPIO26_SRXD_MD);

    printk("Registering SPI board info... ");
    retval = spi_register_board_info(loox720_board_info, ARRAY_SIZE(loox720_board_info));
    printk("done.\n");

    return retval;
}

static void __exit loox720_ts_exit(void)
{
}

// Setup this linux kernel module
module_init(loox720_ts_init);
module_exit(loox720_ts_exit);

MODULE_AUTHOR("Jeroen Dierckx");
MODULE_DESCRIPTION("Touchscreen support for the X50(v), using the ads7846 and pxa2xx_spi SPI drivers");
MODULE_LICENSE("GPL");

#endif
