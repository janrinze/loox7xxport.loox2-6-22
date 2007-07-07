
//#include <linux/init.h>
#include <linux/kernel.h>
//#include <linux/ioport.h>
#include <linux/platform_device.h>
/*#include <linux/input.h>
#include <linux/input_pda.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/gpio_keys.h>
#include <linux/dpm.h>
#include <linux/rs232_serial.h>
#include <asm/gpio.h>
#include <asm/arch/pxa2xx_udc_gpio.h>
*/
#include <asm/mach-types.h>
/*#include <asm/io.h>
#include <asm/hardware.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <asm/arch/serial.h>
#include <asm/arch/hx4700.h>
#include <asm/arch/hx4700-gpio.h>
#include <asm/arch/hx4700-asic.h>
#include <asm/arch/hx4700-core.h>
#include <asm/arch/pxa-regs.h>
#include <asm/hardware/asic3_keys.h>
#include <asm/arch/udc.h>
#include <asm/arch/audio.h>
#include <asm/arch/irda.h>

#include <asm/hardware/ipaq-asic3.h>
#include <linux/soc/asic3_base.h>*/
#include <linux/ads7846.h>
#include <linux/touchscreen-adc.h>
#include <linux/adc_battery.h>

#include "../generic.h"


static struct battery_adc_platform_data loox720_battery_params = {
    .battery_info = {
	.name = "main-battery",
	.voltage_max_design = 1400000,
	.voltage_min_design = 1000000,
	.charge_full_design = 100000,
    },
    .voltage_pin = "ads7846-ssp:vaux",
};

static struct platform_device loox720_battery = { 
    .name = "adc-battery",
    .id = -1,
    .dev = {
	.platform_data = &loox720_battery_params,
    }
};

static int __devinit loox720_battery_probe(struct platform_device *dev)
{
    platform_device_register(&loox720_battery);
    return 0;
}

static struct platform_driver loox720_battery_driver = {
    .driver = {
	.name = "loox720-battery",
    },
    .probe = loox720_battery_probe,
};

static int __init loox720_battery_init(void)
{
    if (!machine_is_loox720())
	return -ENODEV;
    return platform_driver_register(&loox720_battery_driver);
}

static void __exit loox720_battery_exit(void)
{
    platform_driver_unregister(&loox720_battery_driver);
}

module_init(loox720_battery_init);
module_exit(loox720_battery_exit);

MODULE_AUTHOR ("Piotr Czechowicz");
MODULE_DESCRIPTION ("Battery support for FSC Loox 720");
MODULE_LICENSE ("GPL");
