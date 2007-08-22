/* Core Hardware driver for FSC Loox 720
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
#include <asm/arch/loox720-cpld.h>
#include <asm/arch/loox720.h>

#include "loox720_core.h"

/*int
loox720_udc_detect( void )
{
//    return (asic3_get_gpio_status_d(&loox720_asic3.dev)
			& (1 << GPIOD_USBC_DETECT_N)) ? 0 : 1;
    return 1;
}*/

static unsigned int serial_irq = 0xffffffff;
static unsigned int ac_irq = 0xffffffff;

/*static int
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
}*/

static int
ac_isr(int irq, void *data)
{
//	unsigned int statusd;
	int connected;

	if (irq != ac_irq)
	    return IRQ_NONE;

//	statusd = asic3_get_gpio_status_d( &loox720_asic3.dev );
//	connected = (statusd & (1<<GPIOD_AC_IN_N)) == 0;

	connected = GET_LOOX720_GPIO(AC_IN_N) == 0;
	
	printk( KERN_INFO "ac_isr: connected=%d\n", connected );
	if (connected)
	    set_irq_type( ac_irq, IRQT_RISING ); 
	else
	    set_irq_type( ac_irq, IRQT_FALLING ); 

	if (connected)
	    loox720_enable_led(LOOX720_LED_RIGHT, LOOX720_LED_COLOR_B | LOOX720_LED_BLINK);
	else
	    loox720_disable_led(LOOX720_LED_RIGHT, LOOX720_LED_COLOR_B);
	    
//	SET_LOOX720_GPIO_N( CHARGE_EN, connected );
	return IRQ_HANDLED;
}

static int
loox720_core_probe( struct platform_device *pdev )
{
//	unsigned int statusd;
	int connected;
//	struct loox720_core_funcs *funcs = (struct loox720_core_funcs *) pdev->dev.platform_data;
	printk( KERN_NOTICE "Loox 720 Core Hardware Driver\n" );

//	funcs->udc_detect = loox720_udc_detect;
	
/*	egpios = (volatile u_int16_t *)ioremap_nocache( EGPIO_BASE, sizeof *egpios );
	if (!egpios)
		return -ENODEV;
*/
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
*/	
	ac_irq = LOOX720_IRQ(AC_IN_N);
	
	connected = GET_LOOX720_GPIO(AC_IN_N) == 0;
	
	printk( KERN_INFO "AC: connected=%d\n", connected );
        if (request_irq( ac_irq, ac_isr, SA_INTERRUPT,
			    "Loox 720 AC Detect", NULL ) != 0) {
		printk( KERN_ERR "Unable to configure AC detect interrupt.\n" );
		free_irq( serial_irq, NULL );
		return -ENODEV;
	}

	if (connected)
	    set_irq_type( ac_irq, IRQT_RISING ); 
	else
	    set_irq_type( ac_irq, IRQT_FALLING ); 

    if (connected)
        loox720_enable_led(LOOX720_LED_RIGHT, LOOX720_LED_COLOR_B | LOOX720_LED_BLINK);
    else
        loox720_disable_led(LOOX720_LED_RIGHT, LOOX720_LED_COLOR_B);

//	SET_LOOX720_GPIO_N( CHARGE_EN, connected );*/
	return 0;
}

static int
loox720_core_remove( struct platform_device *dev )
{
//	int irq;

//        irq = asic3_irq_base( &loox720_asic3.dev ) + ASIC3_GPIOD_IRQ_BASE
//			+ GPIOD_COM_DCD;
//	if (egpios != NULL)
//		iounmap( (void *)egpios );
		
//	if (serial_irq != 0xffffffff)
//		free_irq( serial_irq, NULL );
	if (ac_irq != 0xffffffff)
		free_irq( ac_irq, NULL );
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

