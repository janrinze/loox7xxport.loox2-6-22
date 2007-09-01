#include <linux/module.h>
#include <linux/version.h>
#include <linux/irq.h>
#include <asm/mach-types.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/loox720-cpld.h>
#include <asm/arch/loox720-gpio.h>
#include <asm/arch/irqs.h>
#include <linux/delay.h>

static u32 reg_cache[8]={
    0, 0, 0, 0, 0, 0, 0, 0
};

int loox720_cpld_irq_base = -1;
EXPORT_SYMBOL(loox720_cpld_irq_base);

static volatile u32 *cpld_mem = 0;

struct cpld_bit
{
    int	bit;
    int value;
};

struct loox720_irq_data
{
	u32 in_service_loop;
	u32 pending_irqs;
	u32 irq_mask;
	int irq_base;
	spinlock_t lock;
	spinlock_t masklock;
	spinlock_t pendinglock;
};

struct loox720_irq_data * loox720_cpld_irq_data;

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

u32	loox720_cpld_reg_read(int regno)
{
	u32 retval = 0;
/*	if(regno>3)
	{
		printk(KERN_INFO "skipping cpld_reg_read(%d)\n", regno);
		return 0;
	} */
	if(cpld_mem) retval = cpld_mem[regno];
    return retval;
}
EXPORT_SYMBOL(loox720_cpld_reg_read);

void	loox720_cpld_reg_write(int regno, u32 value)
{
    if (cpld_mem)
      cpld_mem[regno] = value;
}
EXPORT_SYMBOL(loox720_cpld_reg_write);

u32	loox720_egpio_cache_get(int regno)
{
	if(regno>0)
	    return reg_cache[regno];
	else
		return 0;
}
EXPORT_SYMBOL(loox720_egpio_cache_get);

void	loox720_egpio_cache_set(int regno, u32 value)
{
	if(regno>0)
	   	reg_cache[regno] = value;
}
EXPORT_SYMBOL(loox720_egpio_cache_set);


void	loox720_egpio_cache_enable_bits(int pos, u32 bits)
{
    reg_cache[pos] |= bits;
}

void	loox720_egpio_cache_clear_bits(int pos, u32 bits)
{
    reg_cache[pos] &=~bits;
}


void	loox720_egpio_set_bit(int bit, int val)
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
EXPORT_SYMBOL(loox720_egpio_set_bit);

void	loox720_cpld_ack_irq(unsigned int irq)
{
    struct loox720_irq_data * irq_data = get_irq_chip_data(irq);
	spin_lock(&irq_data->pendinglock);
		irq_data->pending_irqs &= ~(1 << (irq - irq_data->irq_base));
	spin_unlock(&irq_data->pendinglock);
}

void 	loox720_cpld_mask_irq(unsigned int irq)
{
	struct loox720_irq_data * irq_data = get_irq_chip_data(irq);
	spin_lock(&irq_data->masklock);
		irq_data->irq_mask |= 1 << (irq - irq_data->irq_base);
	spin_unlock(&irq_data->masklock);
	spin_lock(&irq_data->lock);
		if(!irq_data->in_service_loop)
			cpld_mem[0] = irq_data->irq_mask << 16;
	spin_unlock(&irq_data->lock);
}

void	loox720_cpld_unmask_irq(unsigned int irq)
{
	struct loox720_irq_data * irq_data = get_irq_chip_data(irq);
	spin_lock(&irq_data->masklock);
		irq_data->irq_mask &= ~(1 << (irq - irq_data->irq_base));
	spin_unlock(&irq_data->masklock);
    spin_lock(&irq_data->lock);
        if(!irq_data->in_service_loop)
            cpld_mem[0] = irq_data->irq_mask << 16;
    spin_unlock(&irq_data->lock);
}

static struct irq_chip loox720_irq_chip = {
    .name       = "CPLD",
    .ack        = loox720_cpld_ack_irq,
    .mask       = loox720_cpld_mask_irq,
    .unmask     = loox720_cpld_unmask_irq,
};

static void loox720_cpld_irq_demux(unsigned int irq, struct irq_desc *desc)
{
    unsigned int mask;
	struct irq_desc * proc_desc;
	unsigned int proc_irq;
	struct loox720_irq_data * irq_data = desc->handler_data;
	unsigned int pending;

	spin_lock(&irq_data->lock);
		irq_data->in_service_loop = 1;
		cpld_mem[0] = 0xFFFF0000;
	spin_unlock(&irq_data->lock);

    do {
        mask = cpld_mem[0] & (~irq_data->irq_mask);
		pending = irq_data->pending_irqs;
		cpld_mem[0] = mask & ~pending;
		spin_lock(&irq_data->pendinglock);
			irq_data->pending_irqs |= mask;
		spin_unlock(&irq_data->pendinglock);
		mask |= pending;
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
    } while (cpld_mem[0] || irq_data->pending_irqs);

	spin_lock(&irq_data->lock);
		irq_data->in_service_loop = 0;
		cpld_mem[0] = irq_data->irq_mask << 16;
	spin_unlock(&irq_data->lock);
}

static int __init loox720_cpld_init(void)
{
    int i;

    if (!machine_is_loox720())
	return -ENODEV;
	loox720_cpld_irq_base = IRQ_BOARD_START;
    printk(KERN_INFO "Loox 720 CPLD Driver\n");

    cpld_mem = (u32*)ioremap(LOOX720_CPLD_PHYS, LOOX720_CPLD_SIZE);
    if (!cpld_mem)
    {
		printk(KERN_ERR "ioremap failed.\n");
		return -ENODEV;
    }

    printk(KERN_INFO "Physical address of CPLD: %08X\nVirtual address of CPLD: %08X\nUsing IRQs from %d to %d on IRQ %d\n",
		LOOX720_CPLD_PHYS, (u32)cpld_mem, loox720_cpld_irq_base, loox720_cpld_irq_base + LOOX720_CPLD_IRQ_COUNT - 1, LOOX720_IRQ(CPLD_INT));

    for (i=0;i<ARRAY_SIZE(loox720_cpld_bits);i++)
    {
		loox720_egpio_set_bit(
		  loox720_cpld_bits[i].bit,
		  loox720_cpld_bits[i].value
		);
    }
	loox720_cpld_irq_data = kmalloc(sizeof(struct loox720_irq_data), GFP_KERNEL);
	if(!loox720_cpld_irq_data)
	{
	    printk(KERN_ERR "kmalloc failed.\n");
	    if (cpld_mem)
	        iounmap(cpld_mem);
        return -ENOMEM;
	}
	loox720_cpld_irq_data->irq_base = loox720_cpld_irq_base;
	loox720_cpld_irq_data->irq_mask = 0xFFFF;
	loox720_cpld_irq_data->in_service_loop = 0;
	loox720_cpld_irq_data->pending_irqs = 0;
	spin_lock_init(&loox720_cpld_irq_data->lock);
	spin_lock_init(&loox720_cpld_irq_data->masklock);
	spin_lock_init(&loox720_cpld_irq_data->pendinglock);
	cpld_mem[0] = cpld_mem[0]; // Mask and clear all interrupts
	for (i=0; i<LOOX720_CPLD_IRQ_COUNT; i++)
	{
		int irq = i + loox720_cpld_irq_base;
        set_irq_chip(irq, &loox720_irq_chip);
		set_irq_chip_data(irq, loox720_cpld_irq_data);
        set_irq_handler(irq, handle_edge_irq);
        set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
	}
    set_irq_chained_handler(LOOX720_IRQ(CPLD_INT), loox720_cpld_irq_demux);
	set_irq_data(LOOX720_IRQ(CPLD_INT), loox720_cpld_irq_data);
    set_irq_type(LOOX720_IRQ(CPLD_INT), IRQT_RISING);

    return 0;
}

static void __exit loox720_cpld_exit(void)
{
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

MODULE_AUTHOR("Piotr Czechowicz & Tomasz Figa");
MODULE_DESCRIPTION("Loox 720 CPLD driver");
MODULE_LICENSE("GPL");
