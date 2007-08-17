/*
 * include/asm/arm/arch-pxa/loox720.h
 */

#ifndef _LOOX720_H_
#define _LOOX720_H_

/* Loox 720 general header
 */

struct loox720_bt_funcs {
        void (*configure) ( int state );
};

#define LOOX720_GPIO_CF_WIFIIRQ      (12)   /* CF Wifi IRQ? */
#define LOOX720_GPIO_CF_DETECT   (13)   /* CF Card Detect? */
#define LOOX720_GPIO_CF_PWR      (14)   /* CF Power? */
#define LOOX720_GPIO_CF_IRQ  (52)   /* CF IRQ? */
#define LOOX720_GPIO_CF0_RESET	(19)	/* CF Wifi Reset? */

/*
 * HX2750 Interrupts
 */
#define LOOX720_IRQ_GPIO_CF_DETECT   IRQ_GPIO(LOOX720_GPIO_CF_DETECT)
#define LOOX720_IRQ_GPIO_CF_IRQ      IRQ_GPIO(LOOX720_GPIO_CF_IRQ)
#define LOOX720_IRQ_GPIO_CF_WIFIIRQ  IRQ_GPIO(LOOX720_GPIO_CF_WIFIIRQ)

#endif /* _LOOX720_H_ */

