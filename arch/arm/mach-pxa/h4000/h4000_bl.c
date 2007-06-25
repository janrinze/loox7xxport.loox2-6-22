/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Copyright (C) 2006 Paul Sokolosvky
 * Based on code from older versions of h4000_lcd.c
 *
 */

#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/corgi_bl.h>
#include <asm/arch/hardware.h>
#include <asm/arch/pxa-regs.h>

#include <asm/arch/h4000-gpio.h>
#include <asm/arch/h4000-asic.h>
#include <linux/soc/asic3_base.h>

#define H4000_MAX_INTENSITY 0x3ff

extern struct platform_device h4000_asic3;

static void h4000_set_bl_intensity(int intensity)
{
	/* LCD brightness is driven by PWM0.
	 * We'll set the pre-scaler to 8, and the period to 1024, this
	 * means the backlight refresh rate will be 3686400/(8*1024) =
	 * 450 Hz which is quite enough.
	 */
	PWM_CTRL0 = 7;            /* pre-scaler */
	PWM_PWDUTY0 = intensity; /* duty cycle */
	PWM_PERVAL0 = H4000_MAX_INTENSITY;      /* period */

	if (intensity > 0) {
		pxa_set_cken(CKEN0_PWM0, 1);
		asic3_set_gpio_out_b(&h4000_asic3.dev,
			GPIOB_BACKLIGHT_POWER_ON, GPIOB_BACKLIGHT_POWER_ON);
	} else {
		pxa_set_cken(CKEN0_PWM0, 0);
		asic3_set_gpio_out_b(&h4000_asic3.dev,
			GPIOB_BACKLIGHT_POWER_ON, 0);
	}
}


static struct corgibl_machinfo h4000_bl_machinfo = {
        .default_intensity = H4000_MAX_INTENSITY / 4,
        .limit_mask = 0xffff,
        .max_intensity = H4000_MAX_INTENSITY,
        .set_bl_intensity = h4000_set_bl_intensity,
};

struct platform_device h4000_bl = {
        .name = "corgi-bl",
        .dev = {
    		.platform_data = &h4000_bl_machinfo,
	},
};

MODULE_AUTHOR("Paul Sokolovsky <pmiscml@gmail.com>");
MODULE_DESCRIPTION("Backlight driver for iPAQ H4000");
MODULE_LICENSE("GPL");

