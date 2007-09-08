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

static unsigned int ac_irq = 0xffffffff;
static unsigned int battery_irq = 0xffffffff;

static void update_battery_charging(void)
{
    int connected = GET_LOOX720_GPIO(AC_IN_N) == 0;
    if (connected)
    {
        int battery_full = GET_LOOX720_GPIO(BATTERY_FULL_N) == 0;
	printk(KERN_INFO "battery: external power: %d; battery_full: %d\n", connected, battery_full);
	if (!battery_full)
	{
	    SET_LOOX720_GPIO_N( CHARGE_EN, 1 );
	    loox720_enable_led(LOOX720_LED_RIGHT, LOOX720_LED_COLOR_B | LOOX720_LED_BLINK);
	}
	else
	{
	    SET_LOOX720_GPIO_N( CHARGE_EN, 0 );
	    loox720_enable_led(LOOX720_LED_RIGHT, LOOX720_LED_COLOR_B);
	}
    }
    else
    {
	printk(KERN_INFO "battery: external power is disconnected.\n");
	SET_LOOX720_GPIO_N( CHARGE_EN, 0 );
        loox720_disable_led(LOOX720_LED_RIGHT, LOOX720_LED_COLOR_B);
    }
}

static int
battery_isr(int irq, void *data)
{
    int full = GET_LOOX720_GPIO(BATTERY_FULL_N) == 0;
    printk( KERN_INFO "battery_isr: battery_full=%d\n", full);
    if (full)
        set_irq_type( battery_irq, IRQT_RISING ); 
    else
        set_irq_type( battery_irq, IRQT_FALLING ); 

    update_battery_charging();
    
    return IRQ_HANDLED;
}

static int
ac_isr(int irq, void *data)
{
	int connected;

	if (irq != ac_irq)
	    return IRQ_NONE;

	connected = GET_LOOX720_GPIO(AC_IN_N) == 0;
	printk( KERN_INFO "ac_isr: connected=%d\n", connected );

	if (connected)
	    set_irq_type( ac_irq, IRQT_RISING ); 
	else
	    set_irq_type( ac_irq, IRQT_FALLING ); 
	    
	update_battery_charging();

	return IRQ_HANDLED;
}

static int
loox720_core_probe( struct platform_device *pdev )
{
	int connected = GET_LOOX720_GPIO(AC_IN_N) == 0;
        int battery_full = GET_LOOX720_GPIO(BATTERY_FULL_N) == 0;

	printk( KERN_NOTICE "Loox 720 Core Hardware Driver\n" );

	ac_irq = LOOX720_IRQ(AC_IN_N);
	battery_irq = LOOX720_IRQ(BATTERY_FULL_N);
	
	printk( KERN_INFO "AC: connected=%d\n", connected );
        if (request_irq( ac_irq, ac_isr, SA_INTERRUPT,
			    "Loox 720 AC Detect", NULL ) != 0) {
		printk( KERN_ERR "Unable to configure AC detect interrupt.\n" );
		return -ENODEV;
	}
	
	if (request_irq( battery_irq, battery_isr, SA_INTERRUPT, "Loox 720 Battery Full", NULL) != 0) 
	{
	    printk( KERN_ERR "Unable to configure battery-full detect interrupt.\n" );
	    free_irq( ac_irq, NULL );
	    return -ENODEV;
	}

	if (connected)
	    set_irq_type( ac_irq, IRQT_RISING ); 
	else
	    set_irq_type( ac_irq, IRQT_FALLING ); 
	    
	if (battery_full)
	    set_irq_type( battery_irq, IRQT_RISING );
	else
	    set_irq_type( battery_irq, IRQT_FALLING );
	
	update_battery_charging();

	return 0;
}

static int
loox720_core_remove( struct platform_device *dev )
{
	if (ac_irq != 0xffffffff)
	    free_irq( ac_irq, NULL );
	if (battery_irq != 0xffffffff)
	    free_irq( battery_irq, NULL );
	return 0;
}

struct platform_driver loox720_core_driver = {
	.driver = {
	    .name     = "loox720-core",
	},
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

