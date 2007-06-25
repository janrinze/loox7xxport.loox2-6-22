/*
 *
 * iPAQ ASIC Device Identifiers
 *
 * Copyright 2002 Hewlett-Packard Company
 *
 * Use consistent with the GNU GPL is permitted,
 * provided that this copyright notice is
 * preserved in its entirety in all copies and derived works.
 *
 * COMPAQ COMPUTER CORPORATION MAKES NO WARRANTIES, EXPRESSED OR IMPLIED,
 * AS TO THE USEFULNESS OR CORRECTNESS OF THIS CODE OR ITS
 * FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 * Author: Jamey Hicks <jamey.hicks@hp.com>
 *
 */

enum ipaq_asic_device_id {
  IPAQ_ASIC2_ADC_DEVICE_ID,
  IPAQ_ASIC2_KEY_DEVICE_ID,
  IPAQ_ASIC2_SPI_DEVICE_ID,
  IPAQ_ASIC2_BACKLIGHT_DEVICE_ID,
  IPAQ_ASIC2_TOUCHSCREEN_DEVICE_ID,
  IPAQ_ASIC2_ONEWIRE_DEVICE_ID,
  IPAQ_ASIC2_BATTERY_DEVICE_ID,
  IPAQ_ASIC2_AUDIO_DEVICE_ID,
  IPAQ_ASIC2_BLUETOOTH_DEVICE_ID,
  IPAQ_ASIC1_MMC_DEVICE_ID, /* h3800 only */
  IPAQ_ASIC3_MMC_DEVICE_ID  /* h3900, h1910 */
};

extern struct soc_device_driver ipaq_asic2_adc_device_driver;
extern struct soc_device_driver ipaq_asic2_key_device_driver;
extern struct soc_device_driver ipaq_asic2_spi_device_driver;
extern struct soc_device_driver ipaq_asic2_backlight_device_driver;
extern struct soc_device_driver ipaq_asic2_touchscreen_device_driver;
extern struct soc_device_driver ipaq_asic2_onewire_device_driver;
extern struct soc_device_driver ipaq_asic2_battery_device_driver;
extern struct soc_device_driver ipaq_asic1_mmc_device_driver;
extern struct soc_device_driver ipaq_asic2_audio_device_driver;

extern struct bus_type asic2_bus_type;
