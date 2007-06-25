/*
 *
 * Definitions for Samsung HAMCOMP Asic Companion chip
 *
 * Copyright 2003 Hewlett-Packard Company.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * COMPAQ COMPUTER CORPORATION MAKES NO WARRANTIES, EXPRESSED OR IMPLIED,
 * AS TO THE USEFULNESS OR CORRECTNESS OF THIS CODE OR ITS
 * FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 * Author: Jamey Hicks
 *
 * Names and values copied from the H2200 Asic Companion specifications:
 * http://www.handhelds.org/platforms/hp/ipaq-h22xx/
 * 
 * History:
 * 
 * See http://cvs.handhelds.org/cgi-bin/viewcvs.cgi/linux/kernel26/include/asm-arm/hardware/ipaq-hamcop.h
 * for the history of changes.
 */

#ifndef _INCLUDE_IPAQ_HAMCOP_H_ 
#define _INCLUDE_IPAQ_HAMCOP_H_

/******* Macros ********/

#define HAMCOP_REGISTER(x,y)	\
	(_HAMCOP_ ## x ## _Base + _HAMCOP_ ## x ## _ ## y)

/******* General stuff *********/

#define HAMCOP_SRAM_Base		0x00000
#define HAMCOP_SRAM_Size		16384

#define HAMCOP_MAP_SIZE			0x100000

/* CPU Interface Control */

#define _HAMCOP_CIF_Base	0x04000
#define _HAMCOP_CIF_IFMODE	0x0000
#define _HAMCOP_CIF_BOOTMODE	0x0002

#define HAMCOP_CIF_IFMODE	HAMCOP_REGISTER(CIF, IFMODE)
#define HAMCOP_CIF_BOOTMODE	HAMCOP_REGISTER(CIF, BOOTMODE)

#define HAMCOP_CIF_IFMODE_SRAM	0x0000	/* boot code read mode */
#define HAMCOP_CIF_IFMODE_VLIO	0x0001


/******* Interrupt controller ********/

#define _HAMCOP_IC_Base		0x04800

#define _HAMCOP_IC_SRCPND	0x00000
#define _HAMCOP_IC_SRCPND0	0x00000
#define _HAMCOP_IC_SRCPND1	0x00004
#define _HAMCOP_IC_INTMSK	0x00008
#define _HAMCOP_IC_INTMSK0	0x00008
#define _HAMCOP_IC_INTMSK1	0x0000c
#define _HAMCOP_IC_PRIORITY	0x00010
#define _HAMCOP_IC_PRIORITY0	0x00010
#define _HAMCOP_IC_PRIORITY1	0x00014
#define _HAMCOP_IC_INTPND	0x00018
#define _HAMCOP_IC_INTPND0	0x00018
#define _HAMCOP_IC_INTPND1	0x0001c
#define _HAMCOP_IC_DREQ		0x00020

#define HAMCOP_IC_SRCPND0	HAMCOP_REGISTER(IC, SRCPND0) /* raw pending interrupts */
#define HAMCOP_IC_SRCPND1	HAMCOP_REGISTER(IC, SRCPND1) /* raw pending interrupts */
#define HAMCOP_IC_INTMSK0	HAMCOP_REGISTER(IC, INTMSK0) /* interrupt mask */
#define HAMCOP_IC_INTMSK1	HAMCOP_REGISTER(IC, INTMSK1) /* interrupt mask */
#define HAMCOP_IC_PRIORITY0	HAMCOP_REGISTER(IC, PRIORITY0) /* interrupt priority control */
#define HAMCOP_IC_PRIORITY1	HAMCOP_REGISTER(IC, PRIORITY1) /* interrupt priority control */
#define HAMCOP_IC_INTPND0	HAMCOP_REGISTER(IC, INTPND0) /* pending interrupts, accounting for mask */
#define HAMCOP_IC_INTPND1	HAMCOP_REGISTER(IC, INTPND1) /* pending interrupts, accounting for mask */
#define HAMCOP_IC_DREQ		HAMCOP_REGISTER(IC, DREQ)   /* DREQ control */

#define _HAMCOP_IC_INT_NFIF  		25       
#define _HAMCOP_IC_INT_KEYPAD		24       
#define _HAMCOP_IC_INT_WDT  		23       
#define _HAMCOP_IC_INT_ADC  		22       
#define _HAMCOP_IC_INT_ADCTS		21
#define _HAMCOP_IC_INT_SDHC		20
#define _HAMCOP_IC_INT_ONEWIRE		19
#define _HAMCOP_IC_INT_ALARM		18
#define _HAMCOP_IC_INT_TICK		17
#define _HAMCOP_IC_INT_DMA		16
#define _HAMCOP_IC_INT_EAR_IN		15
#define _HAMCOP_IC_INT_GPB6		14
#define _HAMCOP_IC_INT_GPB5		13
#define _HAMCOP_IC_INT_GPB4		12
#define _HAMCOP_IC_INT_SD_DETECT_N	11
#define _HAMCOP_IC_INT_JOYPAD_ACTION	10 
#define _HAMCOP_IC_INT_JOYPAD_UP	 9
#define _HAMCOP_IC_INT_JOYPAD_DOWN	 8
#define _HAMCOP_IC_INT_BATT_FLT	 	 7
#define _HAMCOP_IC_INT_JOYPAD_RIGHT	 6
#define _HAMCOP_IC_INT_JOYPAD_LEFT	 5
#define _HAMCOP_IC_INT_APPBUTTON4	 4
#define _HAMCOP_IC_INT_APPBUTTON3	 3
#define _HAMCOP_IC_INT_APPBUTTON2	 2
#define _HAMCOP_IC_INT_APPBUTTON1	 1
#define _HAMCOP_IC_INT_RSTBUTTON	 0

#define IRQ_HAMCOP_NFIF_MASK		(1 << _HAMCOP_IC_INT_NFIF)
#define IRQ_HAMCOP_KEYPAD_MASK		(1 << _HAMCOP_IC_INT_KEYPAD)
#define IRQ_HAMCOP_WDT_MASK		(1 << _HAMCOP_IC_INT_WDT)
#define IRQ_HAMCOP_ADC_MASK		(1 << _HAMCOP_IC_INT_ADC)
#define IRQ_HAMCOP_ADCTS_MASK		(1 << _HAMCOP_IC_INT_ADCTS)
#define IRQ_HAMCOP_SDHC_MASK		(1 << _HAMCOP_IC_INT_SDHC)
#define IRQ_HAMCOP_ONEWIRE_MASK		(1 << _HAMCOP_IC_INT_ONEWIRE)
#define IRQ_HAMCOP_ALARM_MASK		(1 << _HAMCOP_IC_INT_ALARM)
#define IRQ_HAMCOP_TICK_MASK		(1 << _HAMCOP_IC_INT_TICK)
#define IRQ_HAMCOP_DMA_MASK		(1 << _HAMCOP_IC_INT_DMA)
#define IRQ_HAMCOP_EAR_IN_MASK		(1 << _HAMCOP_IC_INT_EAR_IN)
#define IRQ_HAMCOP_SD_DETECT_N		(1 << _HAMCOP_IC_INT_SD_DETECT_N)

#define IRQ_HAMCOP_JOYPAD_MASK		((1 << _HAMCOP_IC_INT_JOYPAD_LEFT)  | \
					 (1 << _HAMCOP_IC_INT_JOYPAD_RIGHT) | \
				   	 (1 << _HAMCOP_IC_INT_JOYPAD_DOWN)  | \
					 (1 << _HAMCOP_IC_INT_JOYPAD_UP)    | \
					 (1 << _HAMCOP_IC_INT_JOYPAD_ACTION))

#define IRQ_HAMCOP_BATT_FLT_MASK	(1 << _HAMCOP_IC_INT_BATT_FLT)

#define IRQ_HAMCOP_BUTTON_MASK		((1 << _HAMCOP_IC_INT_APPBUTTON1) | \
					 (1 << _HAMCOP_IC_INT_APPBUTTON2) | \
				   	 (1 << _HAMCOP_IC_INT_APPBUTTON3) | \
					 (1 << _HAMCOP_IC_INT_APPBUTTON4))


/********* DMA **********/

#define _HAMCOP_DMA_Base		0x04400
#define _HAMCOP_DMA_Size		0x0002f

#define _HAMCOP_DMA_DISRC0		0x0000
#define _HAMCOP_DMA_DISRC1		0x0004
#define _HAMCOP_DMA_DIDST0		0x0008
#define _HAMCOP_DMA_DIDST1		0x000c
#define _HAMCOP_DMA_DCON0		0x0010
#define _HAMCOP_DMA_DCON1		0x0014
#define _HAMCOP_DMA_DSTAT0		0x0018
#define _HAMCOP_DMA_DSTAT1		0x001c
#define _HAMCOP_DMA_DCSRC		0x0020
#define _HAMCOP_DMA_DCDST		0x0024
#define _HAMCOP_DMA_DMASKTRIG		0x0028

#define HAMCOP_DIxxx_INC_LOC_SHIFT	(0)
#define HAMCOP_DIxxx_INC_LOC_MASK	(3<<0)
#define HAMCOP_DISRC_INC_INC		(0<<0)
#define HAMCOP_DISRC_INC_FIXED		(1<<0)
#define HAMCOP_DISRC_AHB		(0<<1)
#define HAMCOP_DISRC_APB		(1<<1)

#define HAMCOP_DIDST_INC_INC		(0<<0)
#define HAMCOP_DIDST_INC_FIXED		(1<<0)
#define HAMCOP_DIDST_AHB		(0<<1)
#define HAMCOP_DIDST_APB		(1<<1)

#define HAMCOP_DMASKTRIG_STOP		(1<<2)
#define HAMCOP_DMASKTRIG_ON		(1<<1)
#define HAMCOP_DMASKTRIG_SWTRIG		(1<<0)

#define HAMCOP_DCON_DEMAND		(0<<10)
#define HAMCOP_DCON_HANDSHAKE		(1<<10)
#define HAMCOP_DCON_SYNC_PCLK		(0<<9)
#define HAMCOP_DCON_SYNC_HCLK		(1<<9)

#define HAMCOP_DCON_INTREQ		(1<<8)

#define HAMCOP_DCON_TSZ			(1<<7)

#define HAMCOP_DCON_SRCSHIFT		(4)
#define HAMCOP_DCON_SRCMASK		(3<<4)
#define HAMCOP_DCON_CH0_SD		(0<<4) /* HAMCOP has one channel */
#define HAMCOP_DCON_CH0_SD_UNSHIFTED	(0)

#define HAMCOP_DCON_HWTRIG		(1<<3)
#define HAMCOP_DCON_AUTORELOAD		(0<<2)
#define HAMCOP_DCON_NORELOAD		(1<<2)

#define HAMCOP_DCON_BYTE		(0<<0)
#define HAMCOP_DCON_HALFWORD		(1<<0)
#define HAMCOP_DCON_WORD		(2<<0)


/********* Clock and Power Management **********/

#define _HAMCOP_CPM_Base		0x04c00
#define _HAMCOP_CPM_LockTime		0x00000 /* defaults to 0x0fff */
#define _HAMCOP_CPM_PllControl		0x00004 /* defaults to 0x2820 */
#define _HAMCOP_CPM_ClockControl	0x00008 /* defaults to 0x03fe */
#define _HAMCOP_CPM_ClockSleep		0x0000c

#define HAMCOP_CPM_LockTime		HAMCOP_REGISTER(CPM, LockTime)
#define HAMCOP_CPM_PllControl		HAMCOP_REGISTER(CPM, PllControl)
#define HAMCOP_CPM_ClockControl		HAMCOP_REGISTER(CPM, ClockControl)
#define HAMCOP_CPM_ClockSleep		HAMCOP_REGISTER(CPM, ClockSleep)

/* upll lock time count value for uclk, ltime > 150us */

#define HAMCOP_CPM_LOCKTIME_MASK    ((1 << 11) -1) 
#define HAMCOP_CPM_PLLCON_MDIV_SHIFT 8 /* main divider control */
#define HAMCOP_CPM_PLLCON_MDIV_WIDTH 8
#define HAMCOP_CPM_PLLCON_PDIV_SHIFT 2 /* pre-divider control */
#define HAMCOP_CPM_PLLCON_PDIV_WIDTH 6
#define HAMCOP_CPM_PLLCON_SDIV_SHIFT 0 /* post divider control */
#define HAMCOP_CPM_PLLCON_SDIV_WIDTH 2

#define HAMCOP_CPM_CLKCON_GPIO_CLKEN	(1 << 9)
#define HAMCOP_CPM_CLKCON_ADC_CLKEN	(1 << 8)
#define HAMCOP_CPM_CLKCON_1WIRE_CLKEN	(1 << 7)
#define HAMCOP_CPM_CLKCON_SD_CLKEN	(1 << 6)
#define HAMCOP_CPM_CLKCON_RTC_CLKEN	(1 << 5)
#define HAMCOP_CPM_CLKCON_LED_CLKEN	(1 << 4)
#define HAMCOP_CPM_CLKCON_NAND_CLKEN	(1 << 3)
#define HAMCOP_CPM_CLKCON_DMAC_CLKEN	(1 << 2)
#define HAMCOP_CPM_CLKCON_KEYPAD_CLKEN	(1 << 1)
#define HAMCOP_CPM_CLKCON_PLLON_CLKEN	(1 << 0)

#define HAMCOP_CPM_CLKSLEEP_XTCON	(1 << 7)
#define HAMCOP_CPM_CLKSLEEP_UCLK_ON	(1 << 3)
#define HAMCOP_CPM_CLKSLEEP_CLKSEL	(1 << 1)  /* 0 = 1/2 MCLK, 1 = MCLK */
#define HAMCOP_CPM_CLKSLEEP_SLEEP	(1 << 0)


/********* NAND Flash Controller **********/

#define _HAMCOP_NF_Base			0x05000
#define _HAMCOP_NF_Size			0x00047

#define _HAMCOP_NF_CONF0		0x0000
#define _HAMCOP_NF_CONT0		0x0008
#define _HAMCOP_NF_CONT1		0x000c
#define _HAMCOP_NF_CMMD 		0x0010
#define _HAMCOP_NF_ADDR0		0x0014
#define _HAMCOP_NF_DATA 		0x001c
#define _HAMCOP_NF_ECCL0		0x0020
#define _HAMCOP_NF_ECCL1		0x0024
#define _HAMCOP_NF_ECCH0		0x0028
#define _HAMCOP_NF_ECCH1		0x002c
#define _HAMCOP_NF_STAT0		0x0030 /* reserved */
#define _HAMCOP_NF_STAT1		0x0034
#define _HAMCOP_NF_SBLK0		0x0038
#define _HAMCOP_NF_SBLK1		0x003c
#define _HAMCOP_NF_EBLK0		0x0040
#define _HAMCOP_NF_EBLK1 		0x0044

#define HAMCOP_NF_CONF0		HAMCOP_REGISTER(NF, CONF0)
#define HAMCOP_NF_CONT0		HAMCOP_REGISTER(NF, CONT0)
#define HAMCOP_NF_CONT1		HAMCOP_REGISTER(NF, CONT1)
#define HAMCOP_NF_CMMD 		HAMCOP_REGISTER(NF, CMMD)
#define HAMCOP_NF_ADDR0		HAMCOP_REGISTER(NF, ADDR0)
#define HAMCOP_NF_DATA 		HAMCOP_REGISTER(NF, DATA)
#define HAMCOP_NF_ECCL0		HAMCOP_REGISTER(NF, ECCL0)
#define HAMCOP_NF_ECCL1		HAMCOP_REGISTER(NF, ECCL1)
#define HAMCOP_NF_ECCH0		HAMCOP_REGISTER(NF, ECCH0)
#define HAMCOP_NF_ECCH1		HAMCOP_REGISTER(NF, ECCH1)
#define HAMCOP_NF_STAT0		HAMCOP_REGISTER(NF, STAT0)
#define HAMCOP_NF_STAT1		HAMCOP_REGISTER(NF, STAT1)
#define HAMCOP_NF_SBLK0		HAMCOP_REGISTER(NF, SBLK0)
#define HAMCOP_NF_SBLK1		HAMCOP_REGISTER(NF, SBLK1)
#define HAMCOP_NF_EBLK0		HAMCOP_REGISTER(NF, EBLK0)
#define HAMCOP_NF_EBLK1		HAMCOP_REGISTER(NF, EBLK1)

#define HAMCOP_NF_CONF0_TACLS            0x7000
#define HAMCOP_NF_CONF0_TACLS_0          0x0000
#define HAMCOP_NF_CONF0_TACLS_1          0x1000
#define HAMCOP_NF_CONF0_TACLS_2          0x2000
#define HAMCOP_NF_CONF0_TACLS_3          0x3000
#define HAMCOP_NF_CONF0_TACLS_4          0x4000
#define HAMCOP_NF_CONF0_TACLS_5          0x5000
#define HAMCOP_NF_CONF0_TACLS_6          0x6000
#define HAMCOP_NF_CONF0_TACLS_7          0x7000
#define HAMCOP_NF_CONF0_TWRPH0           0x0700
#define HAMCOP_NF_CONF0_TWRPH0_0         0x0000
#define HAMCOP_NF_CONF0_TWRPH0_1         0x0100
#define HAMCOP_NF_CONF0_TWRPH0_2         0x0200
#define HAMCOP_NF_CONF0_TWRPH0_3         0x0300
#define HAMCOP_NF_CONF0_TWRPH0_4         0x0400
#define HAMCOP_NF_CONF0_TWRPH0_5         0x0500
#define HAMCOP_NF_CONF0_TWRPH0_6         0x0600
#define HAMCOP_NF_CONF0_TWRPH0_7         0x0700
#define HAMCOP_NF_CONF0_TWRPH1           0x0070
#define HAMCOP_NF_CONF0_TWRPH1_0         0x0000
#define HAMCOP_NF_CONF0_TWRPH1_1         0x0010
#define HAMCOP_NF_CONF0_TWRPH1_2         0x0020
#define HAMCOP_NF_CONF0_TWRPH1_3         0x0030
#define HAMCOP_NF_CONF0_TWRPH1_4         0x0040
#define HAMCOP_NF_CONF0_TWRPH1_5         0x0050
#define HAMCOP_NF_CONF0_TWRPH1_6         0x0060
#define HAMCOP_NF_CONF0_TWRPH1_7         0x0070
#define HAMCOP_NF_CONF0_HW_NCE           0x0008
#define HAMCOP_NF_CONF0_BUSWIDTH         0x0004  /* read-only */
#define HAMCOP_NF_CONF0_BUSWIDTH_8       0x0000  /* 8-bit bus */
#define HAMCOP_NF_CONF0_BUSWIDTH_16      0x0004  /* 16-bit bus */
#define HAMCOP_NF_CONF0_PAGESIZE         0x0002  /* read-only */
#define HAMCOP_NF_CONF0_PAGESIZE_256     0x0000  /* 256-byte pages */
#define HAMCOP_NF_CONF0_PAGESIZE_512     0x0002  /* 512-byte pages */
#define HAMCOP_NF_CONF0_ADDRCYCLE        0x0001  /* read-only */
#define HAMCOP_NF_CONF0_ADDRCYCLE_3      0x0000  /* 3 address cycles */
#define HAMCOP_NF_CONF0_ADDRCYCLE_4      0x0001  /* 4 address cycles */

#define HAMCOP_NF_CONT0_WP_EN            (1 << 12)
#define HAMCOP_NF_CONT0_INITECC          (1 << 11)
#define HAMCOP_NF_CONT0_NFNCE            (1 << 10)
#define HAMCOP_NF_CONT0_LOCKTIGHT        (1 << 3)
#define HAMCOP_NF_CONT0_LOCK             (1 << 2)
#define HAMCOP_NF_CONT0_MODE             (3 << 0)
#define HAMCOP_NF_CONT0_MODE_DISABLE     (0 << 0)
#define HAMCOP_NF_CONT0_MODE_NFMODE      (3 << 0)

#define HAMCOP_NF_CONT1_ENBILLEGALACCINT 0x0010
#define HAMCOP_NF_CONT1_RNBTRANSMODE     0x0001
#define HAMCOP_NF_STAT1_ILLEGALACCESS    0x0040
#define HAMCOP_NF_STAT1_RNB_TRANSDETECT  0x0008
#define HAMCOP_NF_STAT1_FLASH_NCE        0x0004  /* read-only */
#define HAMCOP_NF_STAT1_FLASH_RNB        0x0001  /* read-only */


/********* ADC **********/

#define _HAMCOP_ADC_Base		0x07800
#define _HAMCOP_ADC_Size		0x00014

/********* Watchdog Timer **********/

#define _HAMCOP_WT_Base		0x06000
#define _HAMCOP_WT_Control	0x00000
#define _HAMCOP_WT_Data		0x00004
#define _HAMCOP_WT_Count	0x00008

#define HAMCOP_WT_Control	HAMCOP_REGISTER(WT, Control)
#define HAMCOP_WT_Data		HAMCOP_REGISTER(WT, Data)
#define HAMCOP_WT_Count		HAMCOP_REGISTER(WT, Count)

/********* OWM **********/

#define _HAMCOP_OWM_Base	0x06800
#define _HAMCOP_OWM_Size	0x00014

/********* Secure Digital Interface **********/

#define _HAMCOP_SDI_Base	0x06c00
#define _HAMCOP_SDI_Size	0x0006f

#define _HAMCOP_SDI_CON		0x0000
#define _HAMCOP_SDI_PRE		0x0002
#define _HAMCOP_SDI_CMDARGH	0x0004
#define _HAMCOP_SDI_CMDARGL	0x0006
#define _HAMCOP_SDI_CMDCON	0x0008
#define _HAMCOP_SDI_CMDSTAT	0x000A
#define _HAMCOP_SDI_RSP0H	0x000C
#define _HAMCOP_SDI_RSP0L	0x000E
#define _HAMCOP_SDI_RSP1H	0x0010
#define _HAMCOP_SDI_RSP1L	0x0012
#define _HAMCOP_SDI_RSP2H	0x0014
#define _HAMCOP_SDI_RSP2L	0x0016
#define _HAMCOP_SDI_RSP3H	0x0018
#define _HAMCOP_SDI_RSP3L	0x001A
#define _HAMCOP_SDI_TIMER1	0x001C
#define _HAMCOP_SDI_TIMER2	0x001E
#define _HAMCOP_SDI_BSIZE	0x0020
#define _HAMCOP_SDI_BNUM	0x0022
#define _HAMCOP_SDI_DCON	0x0024
#define _HAMCOP_SDI_DCNT1	0x0026
#define _HAMCOP_SDI_DCNT2	0x0028
#define _HAMCOP_SDI_DSTA	0x002A
#define _HAMCOP_SDI_FSTA	0x002C
#define _HAMCOP_SDI_IMSK1	0x0030
#define _HAMCOP_SDI_IMSK2	0x0032
#define _HAMCOP_SDI_DATA	0x0060

#define HAMCOP_SDI_CON		HAMCOP_REGISTER(SDI, CON)
#define HAMCOP_SDI_PRE		HAMCOP_REGISTER(SDI, PRE)
#define HAMCOP_SDI_CMDARGH	HAMCOP_REGISTER(SDI, CMDARGH)
#define HAMCOP_SDI_CMDARGL	HAMCOP_REGISTER(SDI, CMDARGL)
#define HAMCOP_SDI_CMDCON	HAMCOP_REGISTER(SDI, CMDCON)
#define HAMCOP_SDI_CMDSTAT	HAMCOP_REGISTER(SDI, CMDSTAT)
#define HAMCOP_SDI_RSP0H	HAMCOP_REGISTER(SDI, RSP0H)
#define HAMCOP_SDI_RSP0L	HAMCOP_REGISTER(SDI, RSP0L)
#define HAMCOP_SDI_RSP1H	HAMCOP_REGISTER(SDI, RSP1H)
#define HAMCOP_SDI_RSP1L	HAMCOP_REGISTER(SDI, RSP1L)
#define HAMCOP_SDI_RSP2H	HAMCOP_REGISTER(SDI, RSP2H)
#define HAMCOP_SDI_RSP2L	HAMCOP_REGISTER(SDI, RSP2L)
#define HAMCOP_SDI_RSP3H	HAMCOP_REGISTER(SDI, RSP3H)
#define HAMCOP_SDI_RSP3L	HAMCOP_REGISTER(SDI, RSP3L)
#define HAMCOP_SDI_TIMER1	HAMCOP_REGISTER(SDI, TIMER1)
#define HAMCOP_SDI_TIMER2	HAMCOP_REGISTER(SDI, TIMER2)
#define HAMCOP_SDI_BSIZE	HAMCOP_REGISTER(SDI, BSIZE)
#define HAMCOP_SDI_BNUM		HAMCOP_REGISTER(SDI, BNUM)
#define HAMCOP_SDI_DCON		HAMCOP_REGISTER(SDI, DCON)
#define HAMCOP_SDI_DCNT1	HAMCOP_REGISTER(SDI, DCNT1)
#define HAMCOP_SDI_DCNT2	HAMCOP_REGISTER(SDI, DCNT2)
#define HAMCOP_SDI_DSTA		HAMCOP_REGISTER(SDI, DSTA)
#define HAMCOP_SDI_FSTA		HAMCOP_REGISTER(SDI, FSTA)
#define HAMCOP_SDI_IMSK1	HAMCOP_REGISTER(SDI, IMSK1)
#define HAMCOP_SDI_IMSK2	HAMCOP_REGISTER(SDI, IMSK2)
#define HAMCOP_SDI_DATA		HAMCOP_REGISTER(SDI, DATA)

#define HAMCOP_SDI_CON_CLOCKTYPE_SD   (0 << 0)
#define HAMCOP_SDI_CON_CLOCKTYPE_MMC  (1 << 0)

#define HAMCOP_SDI_TIMER_MAX	0x1fffff
#define HAMCOP_SDIBNUM_MASK	0xfff

/********* GPIO **********/

#define _HAMCOP_GPIO_Base	0x07000

#define _HAMCOP_GPIO_GPA_CON	0x0000 /* reset value is 0x8000 */
#define _HAMCOP_GPIO_GPA_DAT	0x0004 /* gpio A data register, reset value is 0x00000000 */
#define _HAMCOP_GPIO_GPA_PUP	0x0008 /* pull up disable register, reset value is 0x00080000 */

#define HAMCOP_GPIO_GPA_CON	HAMCOP_REGISTER(GPIO, GPA_CON)
#define HAMCOP_GPIO_GPA_DAT	HAMCOP_REGISTER(GPIO, GPA_DAT) /* gpio A data register */
#define HAMCOP_GPIO_GPA_PUP	HAMCOP_REGISTER(GPIO, GPA_PUP) /* pull up disable register */

#define _HAMCOP_GPIO_GPB_CON	0x0010
#define _HAMCOP_GPIO_GPB_DAT	0x0014 
#define _HAMCOP_GPIO_GPB_PUP	0x0018

#define HAMCOP_GPIO_GPB_CON	HAMCOP_REGISTER(GPIO, GPB_CON)
#define HAMCOP_GPIO_GPB_DAT	HAMCOP_REGISTER(GPIO, GPB_DAT) /* gpio B data register */
#define HAMCOP_GPIO_GPB_PUP	HAMCOP_REGISTER(GPIO, GPB_PUP) /* gpio B pull up disable register */

#define HAMCOP_GPIO_GPx_CON_MASK(_n)	(3 << (2 * _n))

#define HAMCOP_GPIO_GPx_CON_MODE_INPUT	0x00
#define HAMCOP_GPIO_GPx_CON_MODE_OUTPUT	0x01

#define HAMCOP_GPIO_GPx_CON_MODE(_n, _v)	\
	(HAMCOP_GPIO_GPx_CON_MODE_ ## _v << (2 * _n))

#define HAMCOP_GPIO_GPB_DAT_SD_DETECT_N	0x08
#define HAMCOP_GPIO_GPB_DAT_EAR_IN	0x80

#define _HAMCOP_GPIO_GPC_CON	0x0020
#define _HAMCOP_GPIO_GPC_DAT	0x0024 
#define _HAMCOP_GPIO_GPC_PUP	0x0028

#define HAMCOP_GPIO_GPC_CON	HAMCOP_REGISTER(GPIO, GPC_CON)
#define HAMCOP_GPIO_GPC_DAT	HAMCOP_REGISTER(GPIO, GPC_DAT) /* gpio C data register */
#define HAMCOP_GPIO_GPC_PUP	HAMCOP_REGISTER(GPIO, GPC_PUP) /* gpio C pull up disable register */

#define _HAMCOP_GPIO_GPD_CON	0x0030
#define _HAMCOP_GPIO_GPD_DAT	0x0034 
#define _HAMCOP_GPIO_GPD_PUP	0x0038

#define HAMCOP_GPIO_GPD_CON	HAMCOP_REGISTER(GPIO, GPD_CON)
#define HAMCOP_GPIO_GPD_DAT	HAMCOP_REGISTER(GPIO, GPD_DAT) /* gpio D data register */
#define HAMCOP_GPIO_GPD_PUP	HAMCOP_REGISTER(GPIO, GPD_PUP) /* gpio D pull up disable register */

	
#define _HAMCOP_GPIO_CLKOUTCON	0x0040
#define HAMCOP_GPIO_CLKOUTCON	HAMCOP_REGISTER(GPIO, CLKOUTCON)

/* GPIO interrupts */
#define _HAMCOP_GPIO_GPAINT0	0x0050
#define _HAMCOP_GPIO_GPAINT1	0x0054
#define _HAMCOP_GPIO_GPBINT0	0x0058
#define _HAMCOP_GPIO_GPBINT1	0x005c
#define HAMCOP_GPIO_GPAINT0	HAMCOP_REGISTER(GPIO, GPAINT0)
#define HAMCOP_GPIO_GPAINT1	HAMCOP_REGISTER(GPIO, GPAINT1)
#define HAMCOP_GPIO_GPBINT0	HAMCOP_REGISTER(GPIO, GPBINT0)
#define HAMCOP_GPIO_GPBINT1	HAMCOP_REGISTER(GPIO, GPBINT1)

#define HAMCOP_GPIO_GPx_INT_MASK(_n)		(0x0f << (4 * _n))
#define HAMCOP_GPIO_GPx_INT_MODE(_n, _v)	((_v) << (4 * _n))

#define HAMCOP_GPIO_GPx_INT_INTLV_LOW		0x00
#define HAMCOP_GPIO_GPx_INT_INTLV_HIGH		0x01
#define HAMCOP_GPIO_GPx_INT_INTLV_RISING	0x02
#define HAMCOP_GPIO_GPx_INT_INTLV_FALLING	0x04
#define HAMCOP_GPIO_GPx_INT_INTLV_BOTHEDGES	0x06
#define HAMCOP_GPIO_GPx_INT_INTEN_DISABLE	0x00
#define HAMCOP_GPIO_GPx_INT_INTEN_ENABLE	0x08



/* GPIO debounce filters */
#define _HAMCOP_GPIO_GPA0FLT0	0x0060
#define _HAMCOP_GPIO_GPA0FLT1	0x0064
#define _HAMCOP_GPIO_GPA1FLT0	0x0068
#define _HAMCOP_GPIO_GPA1FLT1	0x006c
#define _HAMCOP_GPIO_GPA2FLT0	0x0070
#define _HAMCOP_GPIO_GPA2FLT1	0x0074
#define _HAMCOP_GPIO_GPA3FLT0	0x0078
#define _HAMCOP_GPIO_GPA3FLT1	0x007c
#define _HAMCOP_GPIO_GPA4FLT0	0x0080
#define _HAMCOP_GPIO_GPA4FLT1	0x0084
#define _HAMCOP_GPIO_GPA5FLT0	0x0088
#define _HAMCOP_GPIO_GPA5FLT1	0x008c
#define _HAMCOP_GPIO_GPA6FLT0	0x0090
#define _HAMCOP_GPIO_GPA6FLT1	0x0094
#define _HAMCOP_GPIO_GPA7FLT0	0x0098
#define _HAMCOP_GPIO_GPA7FLT1	0x009c
#define _HAMCOP_GPIO_GPB0FLT0	0x00a0
#define _HAMCOP_GPIO_GPB0FLT1	0x00a4
#define _HAMCOP_GPIO_GPB1FLT0	0x00a8
#define _HAMCOP_GPIO_GPB1FLT1	0x00ac
#define _HAMCOP_GPIO_GPB2FLT0	0x00b0
#define _HAMCOP_GPIO_GPB2FLT1	0x00b4
#define _HAMCOP_GPIO_GPB3FLT0	0x00b8
#define _HAMCOP_GPIO_GPB3FLT1	0x00bc
#define _HAMCOP_GPIO_GPB4FLT0	0x00c0
#define _HAMCOP_GPIO_GPB4FLT1	0x00c4
#define _HAMCOP_GPIO_GPB5FLT0	0x00c8
#define _HAMCOP_GPIO_GPB5FLT1	0x00cc
#define _HAMCOP_GPIO_GPB6FLT0	0x00d0
#define _HAMCOP_GPIO_GPB6FLT1	0x00d4
#define _HAMCOP_GPIO_GPB7FLT0	0x00d8
#define _HAMCOP_GPIO_GPB7FLT1	0x00dc

#define HAMCOP_GPIO_GPA0FLT0	HAMCOP_REGISTER(GPIO, GPA0FLT0)
#define HAMCOP_GPIO_GPA0FLT1	HAMCOP_REGISTER(GPIO, GPA0FLT1)
#define HAMCOP_GPIO_GPA1FLT0	HAMCOP_REGISTER(GPIO, GPA1FLT0)
#define HAMCOP_GPIO_GPA1FLT1	HAMCOP_REGISTER(GPIO, GPA1FLT1)
#define HAMCOP_GPIO_GPA2FLT0	HAMCOP_REGISTER(GPIO, GPA2FLT0)
#define HAMCOP_GPIO_GPA2FLT1	HAMCOP_REGISTER(GPIO, GPA2FLT1)
#define HAMCOP_GPIO_GPA3FLT0	HAMCOP_REGISTER(GPIO, GPA3FLT0)
#define HAMCOP_GPIO_GPA3FLT1	HAMCOP_REGISTER(GPIO, GPA3FLT1)
#define HAMCOP_GPIO_GPA4FLT0	HAMCOP_REGISTER(GPIO, GPA4FLT0)
#define HAMCOP_GPIO_GPA4FLT1	HAMCOP_REGISTER(GPIO, GPA4FLT1)
#define HAMCOP_GPIO_GPA5FLT0	HAMCOP_REGISTER(GPIO, GPA5FLT0)
#define HAMCOP_GPIO_GPA5FLT1	HAMCOP_REGISTER(GPIO, GPA5FLT1)
#define HAMCOP_GPIO_GPA6FLT0	HAMCOP_REGISTER(GPIO, GPA6FLT0)
#define HAMCOP_GPIO_GPA6FLT1	HAMCOP_REGISTER(GPIO, GPA6FLT1)
#define HAMCOP_GPIO_GPA7FLT0	HAMCOP_REGISTER(GPIO, GPA7FLT0)
#define HAMCOP_GPIO_GPA7FLT1	HAMCOP_REGISTER(GPIO, GPA7FLT1)
#define HAMCOP_GPIO_GPB0FLT0	HAMCOP_REGISTER(GPIO, GPB0FLT0)
#define HAMCOP_GPIO_GPB0FLT1	HAMCOP_REGISTER(GPIO, GPB0FLT1)
#define HAMCOP_GPIO_GPB1FLT0	HAMCOP_REGISTER(GPIO, GPB1FLT0)
#define HAMCOP_GPIO_GPB1FLT1	HAMCOP_REGISTER(GPIO, GPB1FLT1)
#define HAMCOP_GPIO_GPB2FLT0	HAMCOP_REGISTER(GPIO, GPB2FLT0)
#define HAMCOP_GPIO_GPB2FLT1	HAMCOP_REGISTER(GPIO, GPB2FLT1)
#define HAMCOP_GPIO_GPB3FLT0	HAMCOP_REGISTER(GPIO, GPB3FLT0)
#define HAMCOP_GPIO_GPB3FLT1	HAMCOP_REGISTER(GPIO, GPB3FLT1)
#define HAMCOP_GPIO_GPB4FLT0	HAMCOP_REGISTER(GPIO, GPB4FLT0)
#define HAMCOP_GPIO_GPB4FLT1	HAMCOP_REGISTER(GPIO, GPB4FLT1)
#define HAMCOP_GPIO_GPB5FLT0	HAMCOP_REGISTER(GPIO, GPB5FLT0)
#define HAMCOP_GPIO_GPB5FLT1	HAMCOP_REGISTER(GPIO, GPB5FLT1)
#define HAMCOP_GPIO_GPB6FLT0	HAMCOP_REGISTER(GPIO, GPB6FLT0)
#define HAMCOP_GPIO_GPB6FLT1	HAMCOP_REGISTER(GPIO, GPB6FLT1)
#define HAMCOP_GPIO_GPB7FLT0	HAMCOP_REGISTER(GPIO, GPB7FLT0)
#define HAMCOP_GPIO_GPB7FLT1	HAMCOP_REGISTER(GPIO, GPB7FLT1)

#define HAMCOP_GPIO_GPA_FLT0(_n)   (HAMCOP_REGISTER(GPIO, GPA0FLT0) + (8 * _n))
#define HAMCOP_GPIO_GPA_FLT1(_n)   (HAMCOP_REGISTER(GPIO, GPA0FLT1) + (8 * _n))
#define HAMCOP_GPIO_GPB_FLT0(_n)   (HAMCOP_REGISTER(GPIO, GPB0FLT0) + (8 * _n))
#define HAMCOP_GPIO_GPB_FLT1(_n)   (HAMCOP_REGISTER(GPIO, GPB0FLT1) + (8 * _n))

#define HAMCOP_GPIO_GPxFLT0_SELCLK	    (1 << 0) /* filter clock mask */
#define HAMCOP_GPIO_GPxFLT0_SELCLK_RTC	    (0 << 0) /* use RTC */
#define HAMCOP_GPIO_GPxFLT0_SELCLK_GCLK	    (1 << 0) /* use GCLK */
#define HAMCOP_GPIO_GPxFLT0_FLTEN	    (1 << 1) /* write-only */

#define HAMCOP_GPIO_GPxFLT1_WIDTH_MASK	    0x3fff  /* filter width in clocks */


#define _HAMCOP_GPIO_MON	0x00e0
#define HAMCOP_GPIO_MON		HAMCOP_REGISTER(GPIO, MON)

#define HAMCOP_GPIO_MON_GPA(n)	(1 << (n))
#define HAMCOP_GPIO_MON_GPB(n)	(1 << (8 + (n)))

#define _HAMCOP_GPIO_SPUCR	0x00e4
#define HAMCOP_GPIO_SPUCR	HAMCOP_REGISTER(GPIO, SPUCR)

#define HAMCOP_GPIO_SPUCR_PU_SDDA	(1 << 0) /* SD data pull-up disable */
#define HAMCOP_GPIO_SPUCR_PU_SDCM	(1 << 1) /* SD cmd pull-up disable */
#define HAMCOP_GPIO_SPUCR_PU_fIO	(1 << 2) /* fIO pull-up disable */
#define HAMCOP_GPIO_SPUCR_PU_MD0	(1 << 3) /* MD[7:0] pull-up disable */
#define HAMCOP_GPIO_SPUCR_PU_MD1	(1 << 4) /* MD[15:8] pull-up disable */

/********* RTC **********/

#define _HAMCOP_RTC_Base	0x06400
#define _HAMCOP_RTC_Size	0x0008c

#define _HAMCOP_RTC_RTCCON	0x0040
#define _HAMCOP_RTC_TICNT	0x0044
#define _HAMCOP_RTC_RTCALM	0x0050
#define _HAMCOP_RTC_ALMSEC	0x0054
#define _HAMCOP_RTC_ALMMIN	0x0058
#define _HAMCOP_RTC_ALMHOUR	0x005c
#define _HAMCOP_RTC_ALMDAY	0x0060
#define _HAMCOP_RTC_ALMMON	0x0064
#define _HAMCOP_RTC_ALMYEAR	0x0068
#define _HAMCOP_RTC_RTCRST	0x006c
#define _HAMCOP_RTC_BCDSEC	0x0070
#define _HAMCOP_RTC_BCDMIN	0x0074
#define _HAMCOP_RTC_BCDHOUR	0x0078
#define _HAMCOP_RTC_BCDDAY	0x007c
#define _HAMCOP_RTC_BCDDATE	0x0080
#define _HAMCOP_RTC_BCDMON	0x0084
#define _HAMCOP_RTC_BCDYEAR	0x0088

#define HAMCOP_RTC_RTCON	HAMCOP_REGISTER(RTC, RTCON)   /* reset value: 0x0000 */
#define HAMCOP_RTC_TICNT	HAMCOP_REGISTER(RTC, TICNT)   /* reset value: 0x0000 */
#define HAMCOP_RTC_RTCALM	HAMCOP_REGISTER(RTC, RTCALM)  /* reset value: 0x0000 */
#define HAMCOP_RTC_ALMSEC	HAMCOP_REGISTER(RTC, ALMSEC)  /* reset value: 0x0000 */
#define HAMCOP_RTC_ALMMIN	HAMCOP_REGISTER(RTC, ALMMIN)  /* reset value: 0x0000 */
#define HAMCOP_RTC_ALMHOUR	HAMCOP_REGISTER(RTC, ALMHOUR) /* reset value: 0x0000 */
#define HAMCOP_RTC_ALMDAY	HAMCOP_REGISTER(RTC, ALMDAY)  /* reset value: 0x0001 */
#define HAMCOP_RTC_ALMMON	HAMCOP_REGISTER(RTC, ALMMON)  /* reset value: 0x0001 */
#define HAMCOP_RTC_ALMYEAR	HAMCOP_REGISTER(RTC, ALMYEAR) /* reset value: 0x0000 */
#define HAMCOP_RTC_RTCRST	HAMCOP_REGISTER(RTC, RTCRST)  /* reset value: 0x0000 */
#define HAMCOP_RTC_BCDSEC	HAMCOP_REGISTER(RTC, BCDSEC)  /* reset value: undefined */
#define HAMCOP_RTC_BCDMIN	HAMCOP_REGISTER(RTC, BCDMIN)  /* reset value: undefined */
#define HAMCOP_RTC_BCDDATE	HAMCOP_REGISTER(RTC, BCDDATE) /* reset value: undefined */
#define HAMCOP_RTC_BCDHOUR	HAMCOP_REGISTER(RTC, BCDHOUR) /* reset value: undefined */
#define HAMCOP_RTC_BCDDAY	HAMCOP_REGISTER(RTC, BCDDAY)  /* reset value: undefined */
#define HAMCOP_RTC_BCDMON	HAMCOP_REGISTER(RTC, BCDMON)  /* reset value: undefined */
#define HAMCOP_RTC_BCDYEAR	HAMCOP_REGISTER(RTC, BCDYEAR) /* reset value: undefined */


/********* LED **********/

#define	HAMCOP_NUMBER_OF_LEDS	5

#define _HAMCOP_LED_Base	0x07200
#define _HAMCOP_LED_Size	0x0002f

#define _HAMCOP_LED_LED0CON0	0x00000
#define _HAMCOP_LED_LED0CON1	0x00004
#define _HAMCOP_LED_LED1CON0	0x00008
#define _HAMCOP_LED_LED1CON1	0x0000C
#define _HAMCOP_LED_LED2CON0	0x00010
#define _HAMCOP_LED_LED2CON1	0x00014
#define _HAMCOP_LED_LED3CON0	0x00018
#define _HAMCOP_LED_LED3CON1	0x0001C
#define _HAMCOP_LED_LED4CON0	0x00020
#define _HAMCOP_LED_LED4CON1	0x00024
#define _HAMCOP_LED_LEDPS0	0x00028
#define _HAMCOP_LED_LEDPS1	0x0002c

#define HAMCOP_LED_LEDNCON0_ENB_OFF	(0 << 0)
#define HAMCOP_LED_LEDNCON0_ENB_ON	(1 << 0)
#define HAMCOP_LED_LEDNCON0_INITV_OFF	(0 << 1)
#define HAMCOP_LED_LEDNCON0_INITV_ON	(1 << 1)
#define HAMCOP_LED_LEDNCON0_CMP_SHIFT	4
#define HAMCOP_LED_LEDNCON0_CMP_MASK	(0x1f << 4)
#define HAMCOP_LED_LEDNCON0_CNTEN_OFF	(0 << 9)
#define HAMCOP_LED_LEDNCON0_CNTEN_ON	(1 << 9)
#define HAMCOP_LED_LEDNCON0_CLK_MASK	(1 << 10)
#define HAMCOP_LED_LEDNCON0_CLK_RTC	(0 << 10)
#define HAMCOP_LED_LEDNCON0_CLK_GCLK	(1 << 10)

#define HAMCOP_LED_LEDNCON1_DIV_SHIFT	11
#define HAMCOP_LED_LEDNCON1_DIV_MASK	(0x1f << 11)

#define _HAMCOP_LEDN_CON0(_n)	(_HAMCOP_LED_LED0CON0 + (8 * (_n)))
#define _HAMCOP_LEDN_CON1(_n)	(_HAMCOP_LED_LED0CON1 + (8 * (_n)))
#define HAMCOP_LEDN_CON0(_n)	HAMCOP_REGISTER(LED, LED0CON0 + (8 * (_n)))
#define HAMCOP_LEDN_CON1(_n)	HAMCOP_REGISTER(LED, LED0CON1 + (8 * (_n)))

#define HAMCOP_LED_LEDPS0	HAMCOP_REGISTER(LED, LEDPS0)
#define HAMCOP_LED_LEDPS1	HAMCOP_REGISTER(LED, LEDPS1)

/********* Keypad **********/

#define _HAMCOP_KEYPAD_Base	0x07400

#define _HAMCOP_KEYPAD_DAT	0x00000
#define _HAMCOP_KEYPAD_INTC	0x00004
#define _HAMCOP_KEYPAD_FLT0	0x00008
#define _HAMCOP_KEYPAD_FLT1	0x0000C

#define HAMCOP_KEYPAD_DAT	HAMCOP_REGISTER(KEYPAD, DAT) /* reset value: 0x10 */
#define HAMCOP_KEYPAD_INTC	HAMCOP_REGISTER(KEYPAD, INTC) /* reset value: 0x00 */
#define HAMCOP_KEYPAD_FLT0	HAMCOP_REGISTER(KEYPAD, FLT0) /* reset value: 0x0000 */
#define HAMCOP_KEYPAD_FLT1	HAMCOP_REGISTER(KEYPAD, FLT1) /* reset value: 0x0000 */

#define HAMCOP_KEYPAD_DAT_DAT		(0xf << 0)
#define HAMCOP_KEYPAD_DAT_VALID		(1 << 4)
#define HAMCOP_KEYPAD_DAT_CLEAR		(1 << 5)
#define HAMCOP_KEYPAD_DAT_EN		(1 << 6)

#define HAMCOP_KEYPAD_INTC_INTLV	(7 << 0)
#define HAMCOP_KEYPAD_INTC_INTLV_LOW	(0 << 0)
#define HAMCOP_KEYPAD_INTC_INTLV_HIGH	(1 << 0)
#define HAMCOP_KEYPAD_INTC_INTLV_RE	(2 << 0)
#define HAMCOP_KEYPAD_INTC_INTLV_FE	(4 << 0)
#define HAMCOP_KEYPAD_INTC_INTLV_BOTH_EDGES	(6 << 0)
#define HAMCOP_KEYPAD_INTC_INTEN	(1 << 3)

#define HAMCOP_KEYPAD_FLT0_SELCLK	(1 << 0)
#define HAMCOP_KEYPAD_FLT0_FILEN	(1 << 1)

#define HAMCOP_KEYPAD_FLT1_WIDTH	(0x3fff << 0)


/********* IRQ *************/

#define HAMCOP_IC_IRQ_START		0
#define HAMCOP_IC_IRQ_COUNT		26

#define _IRQ_HAMCOP_RSTBUTTON	 	(0)
#define _IRQ_HAMCOP_APPBUTTON1	 	(1)
#define _IRQ_HAMCOP_APPBUTTON2	 	(2)
#define _IRQ_HAMCOP_APPBUTTON3	 	(3)
#define _IRQ_HAMCOP_APPBUTTON4	 	(4)
#define _IRQ_HAMCOP_JOYPAD_LEFT		(5)
#define _IRQ_HAMCOP_JOYPAD_RIGHT	(6)
#define _IRQ_HAMCOP_BATT_FLT		(7)
#define _IRQ_HAMCOP_JOYPAD_DOWN		(8)
#define _IRQ_HAMCOP_JOYPAD_UP		(9)
#define _IRQ_HAMCOP_JOYPAD_ACTION	(10) 
#define _IRQ_HAMCOP_SD_DETECT_N		(11)
#define _IRQ_HAMCOP_GPB4		(12)
#define _IRQ_HAMCOP_GPB5		(13)
#define _IRQ_HAMCOP_GPB6		(14)
#define _IRQ_HAMCOP_EAR_IN		(15)
#define _IRQ_HAMCOP_DMA			(16)
#define _IRQ_HAMCOP_TICK		(17)
#define _IRQ_HAMCOP_ALARM		(18)
#define _IRQ_HAMCOP_ONEWIRE		(19)
#define _IRQ_HAMCOP_SDHC		(20)
#define _IRQ_HAMCOP_ADCTS		(21)
#define _IRQ_HAMCOP_ADC  		(22)      
#define _IRQ_HAMCOP_WDT  		(23)       
#define _IRQ_HAMCOP_KEYPAD  		(24)       
#define _IRQ_HAMCOP_NFIF  		(25)       

#define HAMCOP_NR_IRQS			HAMCOP_IC_IRQ_COUNT

struct hamcop_platform_data
{
	u16 clocksleep;
	u16 pllcontrol;
};

#endif
