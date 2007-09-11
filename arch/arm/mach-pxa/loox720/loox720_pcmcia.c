/*
 * PCMCIA Support for HP iPAQ hx2750
 *
 * Copyright 2005 Openedhand Ltd.
 *
 * Author: Richard Purdie <richard@o-hand.com>
 *
 * Based on pxa2xx_mainstone.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/device.h>

#include <pcmcia/ss.h>

#include <asm/hardware.h>
#include <asm/irq.h>

#include <asm/arch/pxa-regs.h>
#include <asm/arch/loox720-gpio.h>
#include <asm/arch/loox720-cpld.h>
#include <asm/arch/loox720.h>
#include <linux/platform_device.h>

#include "../../../../drivers/pcmcia/soc_common.h"

static struct pcmcia_irqs irqs[] = {
	{ 1, LOOX720_CPLD_IRQ(CARD_DETECT), "PCMCIA1 CD" }
};

static int loox720_pcmcia_hw_init(struct soc_pcmcia_socket *skt)
{
	/*
	 * Setup default state of GPIO outputs
	 * before we enable them as outputs.
	 */
	GPSR(GPIO48_nPOE) =
		GPIO_bit(GPIO48_nPOE) |
		GPIO_bit(GPIO49_nPWE) |
		GPIO_bit(GPIO50_nPIOR) |
		GPIO_bit(GPIO51_nPIOW) |
		GPIO_bit(GPIO85_nPCE_1) |
		GPIO_bit(GPIO54_nPCE_2);

	pxa_gpio_mode(GPIO48_nPOE_MD);
	pxa_gpio_mode(GPIO49_nPWE_MD);
	pxa_gpio_mode(GPIO50_nPIOR_MD);
	pxa_gpio_mode(GPIO51_nPIOW_MD);
	pxa_gpio_mode(GPIO85_nPCE_1_MD);
	pxa_gpio_mode(GPIO54_nPCE_2_MD);
	pxa_gpio_mode(GPIO79_pSKTSEL_MD);
	pxa_gpio_mode(GPIO55_nPREG_MD);
	pxa_gpio_mode(GPIO56_nPWAIT_MD);
	pxa_gpio_mode(GPIO57_nIOIS16_MD);

	skt->irq = (skt->nr == 1) ? LOOX720_CPLD_IRQ(CF) : LOOX720_CPLD_IRQ(WIFI);
	printk(KERN_INFO "loox720_pcmcia: Using IRQ %d for socket %d.\n", skt->irq, skt->nr);
	return soc_pcmcia_request_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

static void loox720_pcmcia_hw_shutdown(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_free_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

#define GPLR_BIT(n) (GPLR((n)) & GPIO_bit((n)))
#define GPSR_BIT(n) (GPSR((n)) = GPIO_bit((n)))
#define GPCR_BIT(n) (GPCR((n)) = GPIO_bit((n)))

static void loox720_pcmcia_socket_state(struct soc_pcmcia_socket *skt,
				    struct pcmcia_state *state)
{
	if(skt->nr == 1){
		state->detect = (loox720_cpld_reg_read(1) & LOOX720_CPLD_CF_DETECT_N) ? 0 : 1;
		state->ready  = (loox720_cpld_reg_read(1) & LOOX720_CPLD_CF_READY) ? 1 : 0;
	}
	else{
		state->detect = 1;
		state->ready  = ((loox720_cpld_reg_read(1) & (LOOX720_CPLD_WIFI_ENABLED | LOOX720_CPLD_WIFI_READY)) == (LOOX720_CPLD_WIFI_ENABLED | LOOX720_CPLD_WIFI_READY)) ? 1 : 0;
	}

	state->bvd1   = 1;  /* not available */
	state->bvd2   = 1;  /* not available */
	state->vs_3v  = 1;  /* not available */
	state->vs_Xv  = 0;  /* not available */
	state->wrprot = 0;  /* not available */
}

static int loox720_pcmcia_configure_socket(struct soc_pcmcia_socket *skt,
				       const socket_state_t *state)
{
	int ret = 0;

	if(state->flags & SS_RESET) {
		if(skt->nr == 1)
			loox720_egpio_set_bit(LOOX720_CPLD_CF_RESET_N, 0);
		else
			SET_LOOX720_GPIO(WIFI_RST, 1);//loox720_set_egpio(LOOX720_EGPIO_CF1_RESET);
	} else {
		if(skt->nr == 1)
			loox720_egpio_set_bit(LOOX720_CPLD_CF_RESET_N, 1);
		else
			SET_LOOX720_GPIO(WIFI_RST, 0);//loox720_clear_egpio(LOOX720_EGPIO_CF1_RESET);
	}

	/* Apply socket voltage */
	switch (state->Vcc) {
		case 0:
			if(skt->nr == 1)
			{
                loox720_egpio_set_bit(LOOX720_CPLD_CF_3V3_BIT, 0);
                loox720_egpio_set_bit(LOOX720_CPLD_CF_5V_BIT, 0);
			}
			else
			{
				loox720_egpio_set_bit(LOOX720_CPLD_WIFI_POWER, 0);
				SET_LOOX720_GPIO(WIFI_PWR, 0);//loox720_clear_egpio(LOOX720_EGPIO_WIFI_PWR);
				loox720_disable_led(LOOX720_LED_LEFT, LOOX720_LED_COLOR_B);
			}
			break;
		case 50:
		case 33:
			/* Apply power to socket */
			if(skt->nr == 1)
			{
                loox720_egpio_set_bit(LOOX720_CPLD_CF_5V_BIT, 1);
                loox720_egpio_set_bit(LOOX720_CPLD_CF_3V3_BIT, 1);
			}
			else
			{
				loox720_egpio_set_bit(LOOX720_CPLD_WIFI_POWER, 1);
				SET_LOOX720_GPIO(WIFI_PWR, 1);//loox720_set_egpio(LOOX720_EGPIO_WIFI_PWR);
				loox720_enable_led(LOOX720_LED_LEFT, LOOX720_LED_COLOR_B | LOOX720_LED_BLINK);
			}
			break;
		default:
			printk (KERN_ERR "%s: Unsupported Vcc:%d\n", __FUNCTION__, state->Vcc);
			ret = -1;
			break;
	}
	
	return ret;
}

static void loox720_pcmcia_socket_init(struct soc_pcmcia_socket *skt)
{
}

static void loox720_pcmcia_socket_suspend(struct soc_pcmcia_socket *skt)
{
}

static struct pcmcia_low_level loox720_pcmcia_ops = {
	.owner			= THIS_MODULE,
	.hw_init		= loox720_pcmcia_hw_init,
	.hw_shutdown		= loox720_pcmcia_hw_shutdown,
	.socket_state		= loox720_pcmcia_socket_state,
	.configure_socket	= loox720_pcmcia_configure_socket,
	.socket_init		= loox720_pcmcia_socket_init,
	.socket_suspend		= loox720_pcmcia_socket_suspend,
	.nr			= 2,
};

static struct platform_device *loox720_pcmcia_device;

static void loox720_pcmcia_release(struct device *dev)
{
	kfree(loox720_pcmcia_device);
}

static int __init loox720_pcmcia_init(void)
{
	int ret;

	loox720_pcmcia_device = kmalloc(sizeof(*loox720_pcmcia_device), GFP_KERNEL);
	if (!loox720_pcmcia_device)
		return -ENOMEM;
	memset(loox720_pcmcia_device, 0, sizeof(*loox720_pcmcia_device));
	loox720_pcmcia_device->name = "pxa2xx-pcmcia";
	loox720_pcmcia_device->dev.platform_data = &loox720_pcmcia_ops;
	loox720_pcmcia_device->dev.release = loox720_pcmcia_release;

	ret = platform_device_register(loox720_pcmcia_device);
	if (ret)
		kfree(loox720_pcmcia_device);

	return ret;
}

static void __exit loox720_pcmcia_exit(void)
{
	platform_device_unregister(loox720_pcmcia_device);
}

module_init(loox720_pcmcia_init);
module_exit(loox720_pcmcia_exit);

MODULE_AUTHOR("Richard Purdie <richard@o-hand.com>");
MODULE_DESCRIPTION("iPAQ hx2750 PCMCIA Support");
MODULE_LICENSE("GPL");
