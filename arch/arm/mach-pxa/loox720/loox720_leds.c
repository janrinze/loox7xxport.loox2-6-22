#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <asm/arch/loox720-cpld.h>
#include <asm/arch/loox720.h>

extern void loox720_cpld_reg_write(int regno, u32 value);
extern u32 loox720_egpio_cache_get(int regno);
extern void loox720_egpio_cache_set(int regno, u32 value);

static u8 loox720_leds_cache = 0x00;
/*
	Loox 720 leds are driven by CPLD. For each LED, there are 4+1 bits on CPLD.
	
	Bit combination table for LED 1 (left):
	|---|---|---|---|---|----------------------------------------
	| A | B | C | D | X | LED status
	|---|---|---|---|---|----------------------------------------
	| 0 | 0 | 0 | 1 | 1 | Blinking green and blue
	|---|---|---|---|---|----------------------------------------
	| 0 | 1 | 0 | 1 | 1 | Blinking blue
	|---|---|---|---|---|----------------------------------------
	| 0 | 1 | 1 | 1 | 0 | Solid blue
	|---|---|---|---|---|----------------------------------------
	| 0 | 1 | 1 | 1 | 1 | Inverted blinking blue
	|---|---|---|---|---|----------------------------------------
	| 1 | 0 | 0 | 1 | 1 | Blinking green
	|---|---|---|---|---|----------------------------------------
	| 1 | 1 | 1 | 1 | 0 | Solid green
	|---|---|---|---|---|----------------------------------------
	| 1 | 1 | 1 | 1 | 1 | Inverted blinking green
	|---|---|---|---|---|----------------------------------------

	Bit combination table for LED 2 (right):
	|---|---|---|---|---|----------------------------------------
	| A | B | C | D | X | LED status
	|---|---|---|---|---|----------------------------------------
	| 1 | 1 | 0 | 0 | 1 | Blinking green and orange
	|---|---|---|---|---|----------------------------------------
	| 1 | 1 | 1 | 0 | 1 | Blinking orange
	|---|---|---|---|---|----------------------------------------
	| 1 | 0 | 0 | 0 | 0 | Solid orange
	|---|---|---|---|---|----------------------------------------
	| 1 | 0 | 0 | 0 | 1 | Inverted blinking orange
	|---|---|---|---|---|----------------------------------------
	| 0 | 1 | 0 | 0 | 1 | Blinking green
	|---|---|---|---|---|----------------------------------------
	| 0 | 0 | 1 | 0 | 0 | Solid green
	|---|---|---|---|---|----------------------------------------
	| 0 | 0 | 1 | 0 | 1 | Inverted blinking green
	|---|---|---|---|---|----------------------------------------
	| 1 | 0 | 1 | 0 | 0 | Solid red (?!)
	|---|---|---|---|---|----------------------------------------
	| 1 | 0 | 1 | 0 | 1 | Inverted blinking red (?! #2)
	|---|---|---|---|---|----------------------------------------
	
loox720_leds_cache structure:
	bit 0 - led 1 blue
	bit 1 - led 1 green
	bit 2 - led 1 blink
	bit 3 - led 1 inverted blink mode (working with blink bit set)
	bit 4 - led 2 green
	bit 5 - led 2 orange
	bit 6 - led 2 blink
	bit 7 - led 2 inverted blink mode (working with blink bit set)
*/

static void	loox720_update_leds( void )
{
	u32 v = 0;
	u32 mask = 0;
	if(loox720_leds_cache & (LOOX720_LED1_BLUE | LOOX720_LED1_GREEN))
	{
		mask |= LOOX720_CPLD_LED1_BIT_D;
		if(loox720_leds_cache & LOOX720_LED1_BLINK)
			mask |= LOOX720_CPLD_LED1_BLINK;
		else
			mask |= (LOOX720_CPLD_LED1_BIT_C | LOOX720_CPLD_LED1_BIT_B);
		if(loox720_leds_cache & LOOX720_LED1_BLINK2)
			mask |= (LOOX720_CPLD_LED1_BIT_C | LOOX720_CPLD_LED1_BIT_B);
		if(loox720_leds_cache & LOOX720_LED1_GREEN)
			mask |= LOOX720_CPLD_LED1_BIT_A;
		if(loox720_leds_cache & LOOX720_LED1_BLUE)
			mask |= LOOX720_CPLD_LED1_BIT_B;
		if((loox720_leds_cache & (LOOX720_LED1_BLUE | LOOX720_LED1_GREEN)) == (LOOX720_LED1_BLUE | LOOX720_LED1_GREEN))
			mask &= ~(LOOX720_CPLD_LED1_BIT_A | LOOX720_CPLD_LED1_BIT_B);
	}
	if(loox720_leds_cache & LOOX720_LED2_RED)
	{
		if(loox720_leds_cache & LOOX720_LED2_BLINK)
		{
			mask |= LOOX720_CPLD_LED2_BLINK;
			if(!(loox720_leds_cache & LOOX720_LED2_BLINK2))
				mask |= LOOX720_CPLD_LED2_BIT_B;
		}
		if((loox720_leds_cache & LOOX720_LED2_BLINK2) || (!(loox720_leds_cache & LOOX720_LED2_BLINK)))
		{
			if(loox720_leds_cache & LOOX720_LED2_GREEN)
				mask |= LOOX720_CPLD_LED2_BIT_C;
			if(loox720_leds_cache & LOOX720_LED2_ORANGE)
				mask |= LOOX720_CPLD_LED2_BIT_A;
		}
		else
		{
			if(loox720_leds_cache & LOOX720_LED2_ORANGE)
				mask |= LOOX720_CPLD_LED2_BIT_A | LOOX720_CPLD_LED2_BIT_C;
			if(loox720_leds_cache & LOOX720_LED2_GREEN)
				mask &= ~(LOOX720_CPLD_LED2_BIT_C);
		}
	}
	v = (loox720_egpio_cache_get(4) & ~(0xFFFF)) | mask;
	loox720_egpio_cache_set(4, v);
	loox720_cpld_reg_write(4, v);
	loox720_egpio_set_bit(128, 1);
}

void	loox720_enable_led(int led, int mode)
{
	led &= 0x01;
	mode &= 0x0F;
	if(mode & LOOX720_LED_BLINK_ALT)
	{
		mode |= LOOX720_LED_BLINK;
		loox720_leds_cache &= (0xF0 >> (4*led));
	}
	if(~(mode & LOOX720_LED_BLINK))
		loox720_leds_cache &= (0xF0 >> (4*led));
	if(((loox720_leds_cache >> (4*led)) & (LOOX720_LED_BLINK | LOOX720_LED_BLINK_ALT)) != (mode & (LOOX720_LED_BLINK | LOOX720_LED_BLINK_ALT)))
		loox720_leds_cache &= (0xF0 >> (4*led));
	loox720_leds_cache |= (mode << (4*led));
	loox720_update_leds();
}
EXPORT_SYMBOL(loox720_enable_led);

void    loox720_disable_led(int led, int mode)
{
    led &= 0x01;
    mode &= 0x03;
	mode = mode << (4*led);
    loox720_leds_cache &= ~mode;
	if(~(loox720_leds_cache & ((LOOX720_LED_COLOR_A | LOOX720_LED_COLOR_B) << (4*led))))
		loox720_leds_cache &= (0xF0 >> (4*led));
    loox720_update_leds();
}
EXPORT_SYMBOL(loox720_disable_led);

static int __init
loox720_leds_init( void )
{
	printk(KERN_INFO "Loox720 LED Driver\n");
	loox720_update_leds();
	return 0;
}

static void __exit
loox720_leds_exit( void )
{
	loox720_leds_cache = 0x00;
	loox720_update_leds();
}

module_init( loox720_leds_init );
module_exit( loox720_leds_exit );

MODULE_AUTHOR("Tomasz Figa");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("FSC Loox 720 LED driver 2.0");
