/*
 * HTC Blueangel PCMCIA support
 *
 * Copyright (c) 2005 SDG Systems, LLC
 *
 * Based on code from iPAQ h2200
 *   Copyright  2004 Koen Kooi <koen@vestingbar.nl>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/soc-old.h>

#include <asm/hardware.h>
#include <asm/irq.h>

#include <asm/arch/pxa-regs.h>
#include <asm/arch/htcblueangel-gpio.h>
#include <asm/arch/htcblueangel-asic.h>
#include <asm/hardware/ipaq-asic3.h>
#include <linux/soc/asic3_base.h>

#include "../../../../drivers/pcmcia/soc_common.h"

/*
 * CF resources used on blueangel :
 *
 *     pxa_gpio_mode(GPIO_NR_BLUEANGEL_POE_N_MD);
 *     pxa_gpio_mode(GPIO_NR_BLUEANGEL_PWE_N_MD);
 *     pxa_gpio_mode(GPIO_NR_BLUEANGEL_PIOR_N_MD);
 *     pxa_gpio_mode(GPIO_NR_BLUEANGEL_PIOW_N_MD);
 *     pxa_gpio_mode(GPIO_NR_BLUEANGEL_PCE1_N_MD);
 *     pxa_gpio_mode(GPIO_NR_BLUEANGEL_PCE2_N_MD);
 *     pxa_gpio_mode(GPIO_NR_BLUEANGEL_PREG_N_MD);
 *     pxa_gpio_mode(GPIO_NR_BLUEANGEL_PWAIT_N_MD);
 *     pxa_gpio_mode(GPIO_NR_BLUEANGEL_PIOIS16_N_MD);
 */

#if 0
#define _debug(s, args...) printk (KERN_INFO s, ##args)
#define _debug_func(s, args...) _debug ("%s: " s, __FUNCTION__, ##args)
#else
#define _debug(s, args...)
#define _debug_func(s, args...) _debug ("%s: " s, __FUNCTION__, ##args)
#endif

#if 0
static struct pcmcia_irqs blueangel_cd_irq[] = {
	{ 0, 0,  "PCMCIA CD" }	/* fill in IRQ below */
};
#endif

static void
blueangel_cf_reset( int state )
{
	printk("reset\n");
#if 0
	_debug_func ("%d\n", state);
#endif
}

static int
blueangel_pcmcia_hw_init( struct soc_pcmcia_socket *skt )
{

	_debug_func ("\n");

	printk("hw_init\n");
#if 0

	pxa_gpio_mode(GPIO_NR_HX4700_POE_N_MD | GPIO_DFLT_HIGH);
	pxa_gpio_mode(GPIO_NR_HX4700_PWE_N_MD | GPIO_DFLT_HIGH);
	pxa_gpio_mode(GPIO_NR_HX4700_PIOR_N_MD | GPIO_DFLT_HIGH);
	pxa_gpio_mode(GPIO_NR_HX4700_PIOW_N_MD | GPIO_DFLT_HIGH);
	pxa_gpio_mode(GPIO_NR_HX4700_PCE1_N_MD | GPIO_DFLT_HIGH);
	pxa_gpio_mode(GPIO_NR_HX4700_PCE2_N_MD | GPIO_DFLT_HIGH);
	pxa_gpio_mode(GPIO_NR_HX4700_PREG_N_MD | GPIO_DFLT_HIGH);
	pxa_gpio_mode(GPIO_NR_HX4700_PWAIT_N_MD | GPIO_DFLT_HIGH);
	pxa_gpio_mode(GPIO_NR_HX4700_PIOIS16_N_MD | GPIO_DFLT_HIGH);
	pxa_gpio_mode(GPIO_NR_HX4700_PSKTSEL_MD);

	return soc_pcmcia_request_irqs(skt, blueangel_cd_irq, ARRAY_SIZE(blueangel_cd_irq));
#endif
	return 0;
}

/*
 * Release all resources.
 */
static void
blueangel_pcmcia_hw_shutdown( struct soc_pcmcia_socket *skt )
{
	printk("shutdown\n");
#if 0
	_debug_func ("\n");
	soc_pcmcia_free_irqs( skt, blueangel_cd_irq, ARRAY_SIZE(blueangel_cd_irq) );
#endif
}

static void
blueangel_pcmcia_socket_state( struct soc_pcmcia_socket *skt,
    struct pcmcia_state *state )
{

	printk("socket_state\n");
	state->detect = 1;
	state->ready  = 1;
	state->bvd1   = 1;
	state->bvd2   = 1;
	state->wrprot = 0;
	state->vs_3v  = 1;
	state->vs_Xv  = 0;

	_debug( "detect:%d ready:%d vcc:%d\n",
	       state->detect, state->ready, state->vs_3v );
}

static int
blueangel_pcmcia_config_socket( struct soc_pcmcia_socket *skt,
    const socket_state_t *state )
{

	printk("config_socket\n");
	/* Silently ignore Vpp, output enable, speaker enable. */
	_debug_func( "Reset:%d Vcc:%d Astatusc=0x%x\n",
	    (state->flags & SS_RESET) ? 1 : 0, state->Vcc,
	    statusc );

	return 0;
}

/*
 * Enable card status IRQs on (re-)initialisation.  This can
 * be called at initialisation, power management event, or
 * pcmcia event.
 */
static void
blueangel_pcmcia_socket_init(struct soc_pcmcia_socket *skt)
{
	printk("socket_init\n");
#if 0
	_debug_func ("\n");
	blueangel_cf_reset(0);
#endif
}

/*
 * Disable card status IRQs on suspend.
 */
static void
blueangel_pcmcia_socket_suspend( struct soc_pcmcia_socket *skt )
{
	printk("socket_suspend\n");
#if 0
	_debug_func ("\n");
	blueangel_cf_reset(1);
#endif
}

static struct pcmcia_low_level blueangel_pcmcia_ops = {
	.owner          	= THIS_MODULE,
	.nr         		= 1,
	.hw_init        	= blueangel_pcmcia_hw_init,
	.hw_shutdown		= blueangel_pcmcia_hw_shutdown,
	.socket_state		= blueangel_pcmcia_socket_state,
	.configure_socket	= blueangel_pcmcia_config_socket,
	.socket_init		= blueangel_pcmcia_socket_init,
	.socket_suspend		= blueangel_pcmcia_socket_suspend,
};

static struct platform_device blueangel_pcmcia_device = {
	.name			= "pxa2xx-pcmcia",
	.dev			= {
		.platform_data	= &blueangel_pcmcia_ops,
	}
};

static int __init
blueangel_pcmcia_init(void)
{
	_debug_func ("\n");

	return platform_device_register( &blueangel_pcmcia_device );
}

static void __exit
blueangel_pcmcia_exit(void)
{
	platform_device_unregister( &blueangel_pcmcia_device );
}

module_init(blueangel_pcmcia_init);
module_exit(blueangel_pcmcia_exit);

MODULE_AUTHOR("Todd Blumer, SDG Systems, LLC");
MODULE_DESCRIPTION("HTC Blueangel PCMCIA/CF platform-specific driver");
MODULE_LICENSE("GPL");

/* vim600: set noexpandtab sw=8 ts=8 :*/

