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
#include <linux/pm.h>
#include <linux/dpm.h>
#include <asm/arch/pxa-pm_ll.h>

#include "loox720_core.h"

#define LOOX720_RESUME_STRUCT 0xA8024000
#define LOOX720_RESUME_STRUCT_SIZE 0xd8
static u32 save[54];

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
	    loox720_enable_led(LOOX720_LED_RIGHT, LOOX720_LED_COLOR_C | LOOX720_LED_BLINK);
	}
	else
	{
	    SET_LOOX720_GPIO_N( CHARGE_EN, 0 );
	    loox720_enable_led(LOOX720_LED_RIGHT, LOOX720_LED_COLOR_C);
	}
    }
    else
    {
	printk(KERN_INFO "battery: external power is disconnected.\n");
	SET_LOOX720_GPIO_N( CHARGE_EN, 0 );
        loox720_disable_led(LOOX720_LED_RIGHT, LOOX720_LED_COLOR_C);
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

#ifdef CONFIG_PM
static int loox720_suspend(struct platform_device *pdev, pm_message_t state)
{
	/* 0x20c2 is HTC clock value
	 * CLOCK_CDEX_SOURCE		2
	 * CLOCK_CDEX_SPI		0
	 * CLOCK_CDEX_OWM		0
	 *
	 * CLOCK_CDEX_PWM0		0
	 * CLOCK_CDEX_PWM1		0
	 * CLOCK_CDEX_LED0		1
	 * CLOCK_CDEX_LED1		1
	 *
	 * CLOCK_CDEX_LED2		0
	 * CLOCK_CDEX_SD_HOST		0
	 * CLOCK_CDEX_SD_BUS		0
	 * CLOCK_CDEX_SMBUS		0
	 *
	 * CLOCK_CDEX_CONTROL_CX	0
	 * CLOCK_CDEX_EX0		1
	 * CLOCK_CDEX_EX1		0
	 *
	asic3_set_clock_cdex(&loox720_asic3.dev, 0xffff, 0x21c2);

	*egpios = 0;	// turn off all egpio power */

	/*
	 * Note that WEP1 wake up event is used by bootldr to set the
	 * LEDS when power is applied/removed for charging.
	 */
	PWER = PWER_RTC | PWER_GPIO0 | PWER_GPIO1 | PWER_GPIO12 | PWER_WEP1;	// rtc + power + reset + asic3 + wep1
	PFER = PWER_GPIO1;				// Falling Edge Detect
	PRER = PWER_GPIO0 | PWER_GPIO12;		// Rising Edge Detect

	PGSR0 = 0x01308018; // hx4700: 0x080DC01C
	PGSR1 = 0x008F0112; // hx4700: 0x34CF0002
	PGSR2 = 0x0C03C000; // hx4700: 0x0123C18C
	PGSR3 = 0x00000C80; // hx4700: 0x00100202

	/* These next checks are specifically for charging.  We want to enable
	* it if it is already enabled */
	/* Check for charge enable, GPIO 72 */
//	if(GPLR2 & (1 << 8)) {
		/* Set it */
//		PGSR2 |= (1U << 8);
//	} else {
		/* Clear it */
//		PGSR2 &= ~(1U << 8);
//	}
	/* Check for USB_CHARGE_RATE, GPIO 96 */
//	if(GPLR3 & (1 << 0)) {
		/* Set it */
//		PGSR3 |= (1U << 0);
//	} else {
		/* Clear it */
//		PGSR3 &= ~(1U << 0);
//	}

	PCFR = PCFR_GPROD|PCFR_DC_EN|PCFR_GPR_EN|PCFR_OPDE
		|PCFR_FP|PCFR_PI2CEN; /* was 0x1091; */
	/* The 2<<2 below turns on the Power Island state preservation
	 * and counters.  This allows us to wake up bootldr after a
	 * period of time, and it can set the LEDs correctly based on
	 * the power state.  The bootldr turns it off when it's
	 * charged.
	 */
	PSLR=0xc8000000 | (2 << 2);

	return 0;
}

static int loox720_resume(struct platform_device *pdev)
{
	loox720_cpld_resume();
	return 0;
}
#else
	#define loox720_suspend NULL
	#define loox720_resume NULL
#endif

static void
loox720_pxa_ll_pm_suspend(unsigned long resume_addr)
{
	int i;
	u32 csum, tmp, *p;

	/* Save the first 64 words at 0xa8024000. */
	for (p = phys_to_virt(0xa8024000), i = 0; i < 54; i++)
		save[i] = p[i];

	/* Set the first four words at 0xa8024000 to:
	 * resume address; MMU control; TLB base addr; domain id */
	p[0] = resume_addr;

	asm( "mrc\tp15, 0, %0, c1, c0, 0" : "=r" (tmp) );
	p[1] = tmp & ~(0x3987);	    /* mmu off */

	asm( "mrc\tp15, 0, %0, c2, c0, 0" : "=r" (tmp) );
	p[2] = tmp;	/* Shouldn't matter, since MMU will be off. */

	asm( "mrc\tp15, 0, %0, c3, c0, 0" : "=r" (tmp) );
	p[3] = tmp;	/* Shouldn't matter, since MMU will be off. */

	/* Set PSPR to the checksum the HTC bootloader wants to see. */
	for (csum = 0, i = 0; i < 54; i++) {
		tmp = p[i] & 0x1;
		tmp = tmp << 31;
		tmp |= tmp >> 1;
		csum += tmp;
	}

	PSPR = csum;
}

static void
loox720_pxa_ll_pm_resume(void)
{
	int i;
	u32 *p;

	/* Restore the data at 0xa8024000. */
	for (p = phys_to_virt(0xa8024000), i = 0; i < 54; i++)
		p[i] = save[i];

	/* XXX Do we need to flush the cache? */
}

struct pxa_ll_pm_ops loox720_ll_pm_ops = {
	.suspend = loox720_pxa_ll_pm_suspend,
	.resume  = loox720_pxa_ll_pm_resume,
};

static int
loox720_core_probe( struct platform_device *pdev )
{
	int connected = GET_LOOX720_GPIO(AC_IN_N) == 0;
        int battery_full = GET_LOOX720_GPIO(BATTERY_FULL_N) == 0;

	printk( KERN_NOTICE "Loox 720 Core Hardware Driver\n" );

	ac_irq = LOOX720_IRQ(AC_IN_N);
	battery_irq = LOOX720_IRQ(BATTERY_FULL_N);

	printk( KERN_INFO "AC: connected=%d\n", connected );
        if (request_irq( ac_irq, ac_isr, IRQF_DISABLED,
			    "Loox 720 AC Detect", NULL ) != 0) {
		printk( KERN_ERR "Unable to configure AC detect interrupt.\n" );
		return -ENODEV;
	}

	if (request_irq( battery_irq, battery_isr, IRQF_DISABLED, "Loox 720 Battery Full", NULL) != 0)
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

	pxa_pm_set_ll_ops(&loox720_ll_pm_ops);

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
	.suspend  = loox720_suspend,
	.resume   = loox720_resume,
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

