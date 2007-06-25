/*
 *
 * Definitions for H3600 Handheld Computer
 *
 * Copyright 2000-2002 Compaq Computer Corporation.
 * Copyright 2003 Hewlett-Packard Company.
 *
 * Use consistent with the GNU GPL is permitted,
 * provided that this copyright notice is
 * preserved in its entirety in all copies and derived works.
 *
 * COMPAQ COMPUTER CORPORATION MAKES NO WARRANTIES, EXPRESSED OR IMPLIED,
 * AS TO THE USEFULNESS OR CORRECTNESS OF THIS CODE OR ITS
 * FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 * Author: Jamey Hicks.
 *
 * History:
 *
 * 2001-10-??	Andrew Christian   Added support for iPAQ H3800
 *
 */

#ifndef _INCLUDE_H3600_H_
#define _INCLUDE_H3600_H_

#include <asm/ptrace.h>
#include <linux/interrupt.h>

#ifdef CONFIG_ARCH_SA1100
#include <asm/arch-sa1100/ipaqsa.h>
#endif

/* generalized support for HP iPAQ Handheld Computers */
#define machine_is_ipaq() (machine_is_h3100() || machine_is_h3600() || machine_is_h3800() || machine_is_h3900() || machine_is_h5400() || machine_is_h4700() || machine_is_hx2750())

/*
   Machine-independent GPIO definitions
   --- these are common across all current iPAQ platforms
*/

#define GPIO_NR_H3600_NPOWER_BUTTON	(0)   /* Also known as the "off button"  */
#define GPIO_H3600_NPOWER_BUTTON	GPIO_GPIO (0)	/* Also known as the "off button"  */

#define GPIO_H3600_PCMCIA_CD1		GPIO_GPIO (10)
#define GPIO_H3600_PCMCIA_IRQ1		GPIO_GPIO (11)

/* UDA1341 L3 Interface */
#define GPIO_H3600_L3_DATA		GPIO_GPIO (14)
#define GPIO_H3600_L3_MODE		GPIO_GPIO (15)
#define GPIO_H3600_L3_CLOCK		GPIO_GPIO (16)

#define GPIO_H3600_PCMCIA_CD0		GPIO_GPIO (17)
#define GPIO_H3600_SYS_CLK		GPIO_GPIO (19)
#define GPIO_H3600_PCMCIA_IRQ0		GPIO_GPIO (21)

#define GPIO_H3600_COM_DCD		GPIO_GPIO (23)
#define GPIO_H3600_OPT_IRQ		GPIO_GPIO (24)
#define GPIO_H3600_COM_CTS		GPIO_GPIO (25)
#define GPIO_H3600_COM_RTS		GPIO_GPIO (26)

#define IRQ_GPIO_H3600_NPOWER_BUTTON	IRQ_GPIO0
#define IRQ_GPIO_H3600_PCMCIA_CD1	IRQ_GPIO10
#define IRQ_GPIO_H3600_PCMCIA_IRQ1	IRQ_GPIO11
#define IRQ_GPIO_H3600_PCMCIA_CD0	IRQ_GPIO17
#define IRQ_GPIO_H3600_PCMCIA_IRQ0	IRQ_GPIO21
#define IRQ_GPIO_H3600_COM_DCD		IRQ_GPIO23
#define IRQ_GPIO_H3600_OPT_IRQ		IRQ_GPIO24
#define IRQ_GPIO_H3600_COM_CTS		IRQ_GPIO25


#ifndef __ASSEMBLY__

// Define this to see all suspend/resume init/cleanup messages
#define DEBUG_INIT()  \
        if (1) printk(KERN_NOTICE "%s\n", __FUNCTION__)

#define PDEBUG(format,arg...) printk(KERN_DEBUG __FILE__ ":%s - " format "\n", __FUNCTION__ , ## arg )
#define PALERT(format,arg...) printk(KERN_ALERT __FILE__ ":%s - " format "\n", __FUNCTION__ , ## arg )
#define PERROR(format,arg...) printk(KERN_ERR __FILE__ ":%s - " format "\n", __FUNCTION__ , ## arg )


#endif /* ASSEMBLY */

#endif /* _INCLUDE_H3600_H_ */
