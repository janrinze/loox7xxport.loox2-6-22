#
# Makefile for misc devices that really don't fit anywhere else.
#
obj- := misc.o	# Dummy rule to force built-in.o to be made

obj-$(CONFIG_IBM_ASM)		+= ibmasm/
obj-$(CONFIG_HDPU_FEATURES)	+= hdpuftrs/
obj-$(CONFIG_MSI_LAPTOP)     += msi-laptop.o
obj-$(CONFIG_ASUS_LAPTOP)     += asus-laptop.o
obj-$(CONFIG_LKDTM)		+= lkdtm.o
obj-$(CONFIG_TIFM_CORE)       	+= tifm_core.o
obj-$(CONFIG_TIFM_7XX1)       	+= tifm_7xx1.o
obj-$(CONFIG_SGI_IOC4)		+= ioc4.o
obj-$(CONFIG_SONY_LAPTOP)	+= sony-laptop.o


obj-$(CONFIG_MCP_UCB1x00_AUDIO) += ucb1x00-audio.o
obj-$(CONFIG_MCP_UCB1x00_SIMPAD_SWITCHES)+= simpad-switches.o

obj-y				+= adc/
