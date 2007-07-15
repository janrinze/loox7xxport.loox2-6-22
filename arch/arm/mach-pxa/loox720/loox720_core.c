/* Core Hardware driver for x30 (ASIC3, EGPIOs)
 *
 * Authors: Giuseppe Zompatori <giuseppe_zompatori@yahoo.it>
 *
 * based on previews work, see below:
 *
 * Copyright (c) 2005 SDG Systems, LLC
 *
 * 2005-03-29   Todd Blumer             Converted  basic structure to support hx4700
 */

#include <linux/module.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/mach/irq.h>
#include <asm/arch/pxa-regs.h>
#include <asm/arch/loox720-gpio.h>
#include <asm/arch/loox720-asic.h>
#include <asm/arch/loox720-cpld.h>

#include <linux/soc/asic3_base.h>
#include <asm/hardware/ipaq-asic3.h>

#include "loox720_core.h"

#define EGPIO_OFFSET	0
#define EGPIO_BASE	(PXA_CS5_PHYS+EGPIO_OFFSET)
//#define WLAN_OFFSET	0x800000
//#define WLAN_BASE	(PXA_CS5_PHYS+WLAN_OFFSET)
//#define PPSH_OFFSET	0x1000000

volatile u_int16_t *egpios;
u_int16_t egpio_reg;

volatile u_int16_t *cpld_regs;
static u_int16_t cpld_bits = 0xFFFF;

/*
 * may make sense to put egpios elsewhere, but they're here now
 * since they share some of the same address space with the TI WLAN
 *
 * EGPIO register is write-only
 */

void
loox720_egpio_enable( unsigned long bits )
{
	egpio_reg |= bits;
	*egpios = egpio_reg;
}
EXPORT_SYMBOL(loox720_egpio_enable);

void
loox720_egpio_disable( unsigned long bits )
{
	egpio_reg &= ~bits;
	*egpios = egpio_reg;
}
EXPORT_SYMBOL(loox720_egpio_disable);

void
loox720_cpld_enable( u16 bits )
{
	cpld_bits |= bits;
	cpld_regs[0x0F] = cpld_bits;
        printk("cpld_enable:  [%04X] = %04X\n", (u32)(cpld_regs + 0x0F) , (u32)cpld_bits);
}
EXPORT_SYMBOL(loox720_cpld_enable);

void
loox720_cpld_disable( u16 bits )
{
	cpld_bits &= ~bits;
	cpld_regs[0x0F] = cpld_bits;
	printk("cpld_disable: [%04X] = %04X\n", (u32)(cpld_regs + 0x0F), (u32)cpld_bits);
}
EXPORT_SYMBOL(loox720_cpld_disable);

int
loox720_udc_detect( void )
{
	return (asic3_get_gpio_status_d(&loox720_asic3.dev)
			& (1 << GPIOD_USBC_DETECT_N)) ? 0 : 1;
}

static unsigned int serial_irq = 0xffffffff;
static unsigned int ac_irq = 0xffffffff;

static int
serial_isr(int irq, void *data)
{
	unsigned int statusd;
	int connected;

	if (irq != serial_irq)
	    return IRQ_NONE;

	statusd = asic3_get_gpio_status_d( &loox720_asic3.dev );
	connected = (statusd & (1<<GPIOD_COM_DCD)) != 0;
	printk( KERN_INFO "serial_isr: com_dcd=%d\n", connected );
	SET_LOOX720_GPIO( RS232_ON, connected );
	return IRQ_HANDLED;
}

static int
ac_isr(int irq, void *data)
{
	unsigned int statusd;
	int connected;

	if (irq != ac_irq)
	    return IRQ_NONE;

	statusd = asic3_get_gpio_status_d( &loox720_asic3.dev );
	connected = (statusd & (1<<GPIOD_AC_IN_N)) == 0;
	printk( KERN_INFO "ac_isr: connected=%d\n", connected );
	if (connected)
	    set_irq_type( ac_irq, IRQT_RISING ); 
	else
	    set_irq_type( ac_irq, IRQT_FALLING ); 
	SET_LOOX720_GPIO_N( CHARGE_EN, connected );
	return IRQ_HANDLED;
}

static int
loox720_core_probe( struct platform_device *pdev )
{
//	unsigned int statusd;
//	int connected;
//	struct loox720_core_funcs *funcs = (struct loox720_core_funcs *) pdev->dev.platform_data;
	printk( KERN_NOTICE "Loox 720 Core Hardware Driver\n" );

//	funcs->udc_detect = loox720_udc_detect;
	
	egpios = (volatile u_int16_t *)ioremap_nocache( EGPIO_BASE, sizeof *egpios );
	if (!egpios)
		return -ENODEV;

	cpld_regs = (volatile u_int16_t *)(u32)ioremap_nocache( LOOX720_CPLD_PHYS, LOOX720_CPLD_SIZE );
	if (!cpld_regs)
		return -ENODEV;

	/* UART IRQ */
/*        serial_irq = asic3_irq_base( &loox720_asic3.dev ) + ASIC3_GPIOD_IRQ_BASE
                + GPIOD_COM_DCD;
	printk("serial irq: %d\n", serial_irq);
        if (request_irq( serial_irq, serial_isr, SA_INTERRUPT,
			    "Loox 720 Serial", NULL ) != 0) {
		printk( KERN_ERR "Unable to configure serial port interrupt.\n" );
		return -ENODEV;
	}
	set_irq_type( serial_irq, IRQT_RISING ); 
	
        ac_irq = asic3_irq_base( &loox720_asic3.dev ) + ASIC3_GPIOD_IRQ_BASE
                + GPIOD_AC_IN_N;

	statusd = asic3_get_gpio_status_d( &loox720_asic3.dev );
	connected = (statusd & (1<<GPIOD_AC_IN_N)) == 0;
	printk( KERN_INFO "AC: connected=%d\n", connected );
	if (connected)
	    set_irq_type( ac_irq, IRQT_RISING ); 
	else
	    set_irq_type( ac_irq, IRQT_FALLING ); 

        if (request_irq( ac_irq, ac_isr, SA_INTERRUPT,
			    "Loox 720 AC Detect", NULL ) != 0) {
		printk( KERN_ERR "Unable to configure AC detect interrupt.\n" );
		free_irq( serial_irq, NULL );
		return -ENODEV;
	}
	SET_LOOX720_GPIO_N( CHARGE_EN, connected );*/
	return 0;
}

static int
loox720_core_remove( struct platform_device *dev )
{
	int irq;

//        irq = asic3_irq_base( &loox720_asic3.dev ) + ASIC3_GPIOD_IRQ_BASE
//			+ GPIOD_COM_DCD;
	if (egpios != NULL)
		iounmap( (void *)egpios );
		
	if (cpld_regs)
		iounmap( cpld_regs );
		
/*	if (serial_irq != 0xffffffff)
		free_irq( serial_irq, NULL );
	if (ac_irq != 0xffffffff)
		free_irq( ac_irq, NULL );*/
	return 0;
}

struct platform_driver loox720_core_driver = {
	.driver = {
	    .name     = "loox720-core",
	},
//	.bus	  = &platform_bus_type,
	.probe    = loox720_core_probe,
	.remove   = loox720_core_remove,
};

static int __init
loox720_core_init( void )
{
	return platform_driver_register( &loox720_core_driver );
}


static void __exit
loox720_core_exit( void )
{
	platform_driver_unregister( &loox720_core_driver );
}

module_init( loox720_core_init );
module_exit( loox720_core_exit );

MODULE_AUTHOR("Giuseppe Zompatori <giuseppe_zompatori@yahoo.it>");
MODULE_DESCRIPTION("Loox 720 Core Hardware Driver");
MODULE_LICENSE("GPL");

