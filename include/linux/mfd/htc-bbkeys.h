#ifndef _HTC_BBKEYS_H
#define _HTC_BBKEYS_H

#include <linux/gpiodev.h> /* struct gpiodev_ops */

/* Maximum number of keys available */
enum { BBKEYS_MAXKEY = 16 };

struct htc_bbkeys_platform_data {
	/* This is 'gpiodev' capable */
	struct gpiodev_ops ops;

	/* Main interface gpios */
	int sda_gpio, scl_gpio;

	/* Registers for working with keypad */
	int ackreg, key1reg, key2reg;
	/* Keycodes for buttons */
	int buttons[BBKEYS_MAXKEY];
};

#endif
