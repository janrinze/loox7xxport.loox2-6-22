#include <linux/module.h>
#include <linux/version.h>
#include <asm/mach-types.h>
#include <asm/io.h>
#include <asm/arch/loox720-cpld.h>

static u32 reg_cache[8]={
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
};

static u32 *cpld_mem = 0;


void	loox720_cpld_reg_write(int regno, u32 value)
{
    printk(KERN_INFO "cpld_reg_write(%02X, %04X)\n", regno, value);
    cpld_mem[regno] = value;
}


void	loox720_egpio_set_bit(int bit, int val)
{
    int pos = bit/32;
    int sft = bit&31;
    
    if (val)
	reg_cache[pos] |= (u32)(1<<sft);
    else
	reg_cache[pos] &= (u32)~(1<<sft);
	
    loox720_cpld_reg_write(pos, reg_cache[pos]);
}
EXPORT_SYMBOL(loox720_egpio_set_bit);


static int __init loox720_cpld_init(void)
{
    if (!machine_is_loox720())
	return -ENODEV;
	
    printk(KERN_INFO "LOOX720 CPLD Driver\n");
    
    cpld_mem = (u32*)ioremap(LOOX720_CPLD_PHYS, LOOX720_CPLD_SIZE);
    if (!cpld_mem)
    {
	printk(KERN_ERR "ioremap failed.\n");
	return -ENODEV;
    }
    
    printk(KERN_INFO "CPLD PHYS=%08X VIRT=%08X\n", LOOX720_CPLD_PHYS, (u32)cpld_mem);
    
    return 0;
}

static void __exit loox720_cpld_exit(void)
{
	if (cpld_mem)
	    iounmap(cpld_mem);
}

module_init( loox720_cpld_init );
module_exit( loox720_cpld_exit );

MODULE_AUTHOR("Piotr Czechowicz");
