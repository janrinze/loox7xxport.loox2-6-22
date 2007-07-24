/*
 * loox7xx.c  --  SoC audio for Loox 7xx
 *
 * Copyright 2005 Wolfson Microelectronics PLC.
 * Author: Liam Girdwood
 *         liam.girdwood@wolfsonmicro.com or linux@wolfsonmicro.com
 *
 *  Mainstone audio amplifier code taken from arch/arm/mach-pxa/mainstone.c
 *  Copyright:	MontaVista Software Inc.
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  Revision history
 *    30th Oct 2005   Initial version.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <sound/driver.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>

#include <asm/arch/pxa-regs.h>
#include <asm/arch/audio.h>

#include "../codecs/wm8750.h"
#include "pxa2xx-pcm.h"
#include "pxa2xx-i2s.h"

#include <asm/arch/loox720-cpld.h>

static struct snd_soc_machine loox;

static int loox_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec_dai *codec_dai = rtd->dai->codec_dai;
	struct snd_soc_cpu_dai *cpu_dai = rtd->dai->cpu_dai;
	unsigned int clk = 0;
	int ret = 0;

	switch (params_rate(params)) {
	case 8000:
	case 16000:
	case 48000:
	case 96000:
		clk = 12288000;
		break;
	case 11025:
	case 22050:
	case 44100:
	case 88200:
		clk = 11289600;
		break;
	}

	/* set codec DAI configuration */
	ret = codec_dai->dai_ops.set_fmt(codec_dai, SND_SOC_DAIFMT_I2S |
		SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0)
		return ret;

	/* set cpu DAI configuration */
	ret = cpu_dai->dai_ops.set_fmt(cpu_dai, SND_SOC_DAIFMT_I2S |
		SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0)
		return ret;

	/* set the codec system clock for DAC and ADC */
	ret = codec_dai->dai_ops.set_sysclk(codec_dai, WM8750_SYSCLK, clk,
		SND_SOC_CLOCK_IN);
	if (ret < 0)
		return ret;

	/* set the I2S system clock as input (unused) */
	ret = cpu_dai->dai_ops.set_sysclk(cpu_dai, PXA2XX_I2S_SYSCLK, 0,
		SND_SOC_CLOCK_IN);
	if (ret < 0)
		return ret;

	return 0;
}

static int loox_wm8750_init(struct snd_soc_codec *codec)
{
	return 0;
}

static struct snd_soc_ops loox_ops = {
//	.startup = loox_startup,
	.hw_params = loox_hw_params,
};

static struct snd_soc_dai_link loox_dai = {
	.name = "WM8750",
	.stream_name = "WM8750 HiFi",
	.cpu_dai = &pxa_i2s_dai,
	.codec_dai = &wm8750_dai,
	.init = loox_wm8750_init,
	.ops = &loox_ops,
};

static struct snd_soc_machine loox = {
	.name = "Loox Audio",
	.dai_link = &loox_dai,
	.num_links = 1,
};

static struct wm8750_setup_data loox_wm8750_setup = {
	.i2c_address = 0x1a,
};

static struct snd_soc_device loox_snd_devdata = {
	.machine = &loox,
	.platform = &pxa2xx_soc_platform,
	.codec_dev = &soc_codec_dev_wm8750,
	.codec_data = &loox_wm8750_setup,
};

static struct platform_device *loox_snd_device;

static int __init loox_init(void)
{
	int ret;
	
	loox720_egpio_set_bit(LOOX720_CPLD_SOUND_BIT, 1);

	loox_snd_device = platform_device_alloc("soc-audio", -1);
	if (!loox_snd_device)
		return -ENOMEM;

	platform_set_drvdata(loox_snd_device, &loox_snd_devdata);
	loox_snd_devdata.dev = &loox_snd_device->dev;
	ret = platform_device_add(loox_snd_device);

	if (ret)
		platform_device_put(loox_snd_device);

	return ret;
}

static void __exit loox_exit(void)
{
	platform_device_unregister(loox_snd_device);
}

module_init(loox_init);
module_exit(loox_exit);

/* Module information */
MODULE_AUTHOR("Liam Girdwood, liam.girdwood@wolfsonmicro.com, www.wolfsonmicro.com");
MODULE_DESCRIPTION("ALSA SoC Loox");
MODULE_LICENSE("GPL");
