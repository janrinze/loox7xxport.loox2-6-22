#include <linux/module.h>
#include <linux/version.h>
#include <linux/irq.h>
#include <asm/mach-types.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/irqs.h>
#include <linux/delay.h>
#include <asm/arch/loox720-gpio.h>
#include <asm/arch/loox720-cpld.h>

static u32 reg_cache[16]={
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

static volatile u16 *cpld_mem = 0;
int loox720_cpld_irq_base = -1;
EXPORT_SYMBOL(loox720_cpld_irq_base);


struct loox720_irq_data
{
	u32 in_service_loop;
	u32 pending_irqs;
	u32 irq_mask;
	unsigned char individual_mask[32];
	int irq_base;
	spinlock_t lock;
	spinlock_t masklock;
};

struct loox720_irq_data * loox720_cpld_irq_data;

struct cpld_bit
{
    int	bit;
    int value;
};

static struct cpld_bit loox720_cpld_bits[] = 
{
    {
	.bit   = LOOX720_CPLD_SERIAL_BIT,
	.value = 1
    },
    {
	.bit   = LOOX720_CPLD_LCD_CONSOLE_BIT,
	.value = 1
    },
    {
	.bit   = LOOX720_CPLD_LCD_BIT,
	.value = 1
    },
    {
	.bit   = LOOX720_CPLD_LCD_COLOR_BIT,
	.value = 1
    },
    {
	.bit   = LOOX720_CPLD_LCD_BIT2,
	.value = 1
    },
    {
	.bit   = LOOX720_CPLD_BACKLIGHT_BIT,
	.value = 1
    },
    {
	.bit   = LOOX720_CPLD_SD_BIT,
	.value = 1
    }
};

u32 loox720_cpld_reg_read(int regno)
{
    u32 retval = 0;
    if(cpld_mem) retval = cpld_mem[regno];
    return retval;
}
EXPORT_SYMBOL(loox720_cpld_reg_read);

/*
    if we want to know if specific bits are set ..
    this returns them either from the cache or from the real cpld
*/

u32 loox720_cpld_reg_test(int regno, u32 mask)
{
    u32 retval = 0;
    if (regno < 4) // the lower 4 are inputs
    {
	if (cpld_mem)
	    retval = cpld_mem[regno] & mask;
    } else {
	retval = reg_cache[regno] & mask;
    }
    return retval;
}
EXPORT_SYMBOL(loox720_cpld_reg_test);

void loox720_cpld_reg_write(int regno, u32 value)
{
    if (cpld_mem)
      cpld_mem[regno] = (u16) value;
    /*
       apparently we write from outside the cpld environment
       this would cause that bits are not updated in the cache..
       also the change from 32 to 16 bit makes that all code needs
       to be checked for writes directly to the cpld 
    */
    reg_cache[regno] = value;	
}
EXPORT_SYMBOL(loox720_cpld_reg_write);

void loox720_cpld_write_masked(int regno, u32 mask, u32 value)
{
    u32 new_value = (reg_cache[regno] & ~mask ) | (value & mask) ;
    /*
	only the corresponding bits in the mask will be updated
	to the ones from the input value
    */
    if (cpld_mem)
      cpld_mem[regno] =(u16) new_value;
    /*
       apparently we write from outside the cpld environment
       this would cause that bits are not updated in the cache..
       also the change from 32 to 16 bit makes that all code needs
       to be checked for writes directly to the cpld 
    */
    reg_cache[regno] = new_value;	
}
EXPORT_SYMBOL(loox720_cpld_write_masked);

/*
    exporting access to the cached bits is an example of very bad progamming..
    the code remains until Piotr decides that it must go.. 
    20/09/2007 Jan Rinze Peterzon.

u32 loox720_egpio_cache_get(int regno)
{
    if(regno>0)
        return reg_cache[regno];
    else
    	return 0;
}
EXPORT_SYMBOL(loox720_egpio_cache_get);

    
void loox720_egpio_cache_set(int regno, u32 value)
{
    if(regno>0)
	reg_cache[regno] = value;
}
EXPORT_SYMBOL(loox720_egpio_cache_set);
*/

/* 
    this code is a bit archaic i.m.h.o.
void loox720_egpio_cache_enable_bits(int pos, u32 bits)
{
    reg_cache[pos] |= bits;
}

void loox720_egpio_cache_clear_bits(int pos, u32 bits)
{
    reg_cache[pos] &=~bits;
}

void loox720_egpio_set_bit(int bit, int val)
{
    int pos = BIT_POS(bit);
    if(bit > 31)
	{
	    if (val)
		loox720_egpio_cache_enable_bits(pos, BIT_MSK(bit));
	    else
		loox720_egpio_cache_clear_bits(pos, BIT_MSK(bit));
	    loox720_cpld_reg_write(pos, reg_cache[pos]);
	}
}
*/

void loox720_egpio_set_bit(int bit, int val)
{
    int pos = BIT_POS(bit);
    /*
	pos 0 has interrupt bits.
	there is no harm in writing to them if intended
	since that will clear those interrupts.
	But we have chosen not to bother with this..
	pos 1 has interrupt mask register which seems logical to keep
	in the cache.
	
	we actually should be locking interrupts around this..
	there may be race-conditions around setting bits..
	
    */
    if(pos > 0)
    {
	u32 curr_value = reg_cache[pos];
	if (val)
		curr_value |= BIT_MSK(bit);
	    else
		curr_value &=~BIT_MSK(bit);
	    loox720_cpld_reg_write(pos, curr_value);
	}
}
EXPORT_SYMBOL(loox720_egpio_set_bit);

void	loox720_cpld_ack_irq(unsigned int irq)
{
	/*
	    we don't ack irqs.
	    a possibility would be to clear the specific interrupt.
	    we tried but it had no good effects.
	*/
}

void 	loox720_cpld_mask_irq(unsigned int irq)
{
	struct loox720_irq_data * irq_data = get_irq_chip_data(irq);
	/* masking like this is racy.. */
	irq_data->irq_mask |= 1 << (irq - irq_data->irq_base);
	/*
	    NOTE:
	    since the isr only uses the copy we might try and
	    really disable this irq in the cpld:
	*/
	cpld_mem[1] = irq_data->irq_mask;
	/*
	*/
}

void	loox720_cpld_unmask_irq(unsigned int irq)
{
	struct loox720_irq_data * irq_data = get_irq_chip_data(irq);
	/* masking like this is racy.. */
	irq_data->irq_mask &= ~(1 << (irq - irq_data->irq_base));
	/*
	    NOTE:
	    should we bother to clear the irq first?
	    for the people who like to know how:

	    cpld_mem[0] = 1 << (irq - irq_data->irq_base;
	*/
	/*
	    NOTE:
	    since the isr only uses the copy we might try and
	    really disable this irq in the cpld:
	*/
	cpld_mem[1] = irq_data->irq_mask;
	/*
	*/
}

static struct irq_chip loox720_irq_chip = {
    .name       = "CPLD",
    .mask       = loox720_cpld_mask_irq,
    .unmask     = loox720_cpld_unmask_irq,
};

static /*unsigned int*/ void loox720_cpld_irq_demux(unsigned int irq, struct irq_desc *desc)
{
    unsigned int mask;
	struct irq_desc * proc_desc;
	unsigned int proc_irq;
	struct loox720_irq_data * irq_data = desc->handler_data;

    /* the following is racy.. we should add irq disabling around it */
    mask = (cpld_mem[0] & (~irq_data->irq_mask));
	cpld_mem[0] = mask;// ack all to be handled irqs
        if (mask) {
            proc_irq = irq_data->irq_base;
            proc_desc = irq_desc + proc_irq;
            do {
                if (mask & 1)
                    desc_handle_irq(proc_irq, proc_desc);
                proc_irq++;
                proc_desc++;
                mask >>= 1;
            } while (mask);
	}
	cpld_mem[1] = 0x03FF;
	cpld_mem[1] = irq_data->irq_mask;
}

void loox720_cpld_resume(void)
{
	int i;
	for(i = 8; i < 16; i++) {
		loox720_cpld_reg_write(i, reg_cache[i]);
	}
}
EXPORT_SYMBOL(loox720_cpld_resume);

static int __init loox720_cpld_init(void)
{
    int i;

    if (!machine_is_loox720())
	return -ENODEV;

	loox720_cpld_irq_base = IRQ_BOARD_START;
    printk(KERN_INFO "Loox 720 CPLD Driver\n");

    cpld_mem = (u16*)ioremap_nocache(LOOX720_CPLD_PHYS, LOOX720_CPLD_SIZE);
    if (!cpld_mem)
    {
		printk(KERN_ERR "ioremap failed.\n");
		return -ENODEV;
    }

    printk(KERN_INFO "Physical address of CPLD: %08X\nVirtual address of CPLD: %08X\nUsing IRQs from %d to %d on IRQ %d\n",
		LOOX720_CPLD_PHYS, (u32)cpld_mem, loox720_cpld_irq_base, loox720_cpld_irq_base + LOOX720_CPLD_IRQ_COUNT - 1, LOOX720_IRQ(CPLD_INT));

    for (i=0;i<ARRAY_SIZE(loox720_cpld_bits);i++)
    	loox720_egpio_set_bit( loox720_cpld_bits[i].bit, loox720_cpld_bits[i].value);
	loox720_cpld_resume();
    
    loox720_cpld_irq_data = kmalloc(sizeof(struct loox720_irq_data), GFP_KERNEL);
    if(!loox720_cpld_irq_data)
    {
	printk(KERN_ERR "kmalloc failed.\n");
	if (cpld_mem)
	    iounmap(cpld_mem);
        return -ENOMEM;
    }
    
    loox720_cpld_irq_data->irq_base = loox720_cpld_irq_base;
    loox720_cpld_irq_data->irq_mask = 0x03FF;
    loox720_cpld_irq_data->in_service_loop = 0;
    spin_lock_init(&loox720_cpld_irq_data->lock);
    spin_lock_init(&loox720_cpld_irq_data->masklock);
    
    cpld_mem[1] = 0x03FF; // Mask all interrupts

    for (i=0; i<LOOX720_CPLD_IRQ_COUNT; i++)
    {
    	int irq = i + loox720_cpld_irq_base;
        set_irq_chip(irq, &loox720_irq_chip);
    	set_irq_chip_data(irq, loox720_cpld_irq_data);
    	set_irq_handler(irq, handle_simple_irq);
    	set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
    }
    
    set_irq_data(LOOX720_IRQ(CPLD_INT), loox720_cpld_irq_data);
    set_irq_type(LOOX720_IRQ(CPLD_INT), IRQT_RISING);
    set_irq_chained_handler(LOOX720_IRQ(CPLD_INT), loox720_cpld_irq_demux);
    
    cpld_mem[0] = 0x03FF; // clear interrupts
    return 0;
}

static void __exit loox720_cpld_exit(void)
{
    if (cpld_mem)
    {
	cpld_mem[1] = 0x03FF; // Mask all interrupts
	cpld_mem[0] = 0x03FF; // Clear all interrupts
    }
	
    if (loox720_cpld_irq_base!=-1)
    {
        int i;
        for (i = 0 ; i < LOOX720_CPLD_IRQ_COUNT ; i++)
    	{
            int irq = i + loox720_cpld_irq_base;
            set_irq_handler(irq, NULL);
	    set_irq_chip_data(irq, NULL);
            set_irq_chip(irq, NULL);
	    set_irq_flags(irq, 0);
	}
	set_irq_chained_handler(LOOX720_IRQ(CPLD_INT), NULL);
	set_irq_data(LOOX720_IRQ(CPLD_INT), NULL);
    }
    if (loox720_cpld_irq_data)
	kfree(loox720_cpld_irq_data);
    if (cpld_mem)
        iounmap(cpld_mem);
}

module_init( loox720_cpld_init );
module_exit( loox720_cpld_exit );

MODULE_AUTHOR("Piotr Czechowicz & Tomasz Figa & Jan Rinze Peterzon");
MODULE_DESCRIPTION("Loox 720 CPLD driver");
MODULE_LICENSE("GPL");
