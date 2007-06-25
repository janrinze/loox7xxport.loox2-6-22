/*
 *
 * Definitions for H3600 Handheld Computer
 *
 * Copyright 2000 Compaq Computer Corporation.
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
 * 2001-10-??   Andrew Christian   Added support for iPAQ H3800
 * 2005-04-02   Holger Hans Peter Freyther migrate to own file for the 2.6 port
 *
 */

#ifndef _INCLUDE_IPAQSA_H_
#define _INCLUDE_IPAQSA_H_

/* Physical memory regions corresponding to chip selects */
#define IPAQSA_EGPIO_PHYS     0x49000000
#define IPAQSA_BANK_2_PHYS    0x10000000
#define IPAQSA_BANK_4_PHYS    0x40000000


/* Virtual memory regions corresponding to chip selects 2 & 4 (used on sleeves) */
#define IPAQSA_EGPIO_VIRT     0xf0000000
#define IPAQSA_BANK_2_VIRT    0xf1000000
#define IPAQSA_BANK_4_VIRT    0xf3800000

#include <asm/hardware/ipaq-ops.h>

#endif
