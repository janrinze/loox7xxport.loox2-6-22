#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <asm/arch/loox720-cpld.h>

void loox720_cpld_reg_write(int regno, u32 value);
extern u32 loox720_egpio_cache_get(int regno);
extern void loox720_egpio_cache_set(int regno, u32 value);

void	loox720_set_leds(int mode)
{
    u32 v = loox720_egpio_cache_get(4) & ~(0x4000 | 0x300 | 0xF0);
    
    v |= 0x0ff02300; // TODO: needs work...
    
    if (mode & LOOX720_LED2_BLINK)
      v |= 0x4000;
    else
      v &= ~0x4000;
      
    switch ( mode & (LOOX720_LED2_COLOR_GREEN | LOOX720_LED2_COLOR_RED | LOOX720_LED2_COLOR_ORANGE | LOOX720_LED2_BLINK2) )
    {
	case 0: break;
	
	case LOOX720_LED2_COLOR_ORANGE:
	case LOOX720_LED2_COLOR_ORANGE|LOOX720_LED2_BLINK2: v|= 0x10; break;

	case LOOX720_LED2_COLOR_GREEN|LOOX720_LED2_COLOR_ORANGE: v |= 0x30; break;
	
	case LOOX720_LED2_COLOR_GREEN: 
	case LOOX720_LED2_COLOR_GREEN|LOOX720_LED2_BLINK2: v |= 0x40; break;
	
	case LOOX720_LED2_COLOR_RED:
	case LOOX720_LED2_COLOR_RED | LOOX720_LED2_BLINK: v |= 0x50; break;
	
	case LOOX720_LED2_COLOR_ORANGE|LOOX720_LED2_BLINK: v |= 0x70; break;
	
	default: printk(KERN_INFO "invalid mode for LED2 (%04X).\n", mode);
    }
    loox720_egpio_cache_set(4, v);
    loox720_cpld_reg_write(4, v);
}
EXPORT_SYMBOL(loox720_set_leds);


static int __init
loox720_leds_init( void )
{
	printk(KERN_INFO "Loox720 LED Driver\n");
	return 0;
}


static void __exit
loox720_leds_exit( void )
{
}

module_init( loox720_leds_init );
module_exit( loox720_leds_exit );

MODULE_AUTHOR("Piotr Czechowicz");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("FSC Loox 720 LED driver");
