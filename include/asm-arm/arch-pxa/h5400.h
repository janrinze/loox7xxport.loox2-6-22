#include <linux/leds.h>

extern void (*h5400_btuart_configure)(int state);
extern void (*h5400_hwuart_configure)(int state);

EXTERN_LED_TRIGGER_SHARED(h5400_radio_trig);
