#include <linux/irq.h>
#include <linux/module.h>
#include <linux/spinlock.h>

static spinlock_t irq_alloc_lock = SPIN_LOCK_UNLOCKED;

int alloc_irq_space (int nr)
{
	int i = 0, c = 0;

	spin_lock (&irq_alloc_lock);

	while (c < nr && i < NR_IRQS) {
//		if (irq_desc[i].reserved || irq_desc[i].valid)
		if ((irq_desc[i].status & IRQ_RESERVED) || irq_desc[i].handle_irq != handle_bad_irq)
			c = 0;
		else
			c++;
		i++;
	}

	if (c < nr) {
		spin_unlock (&irq_alloc_lock);
		return -1;
	}

	i -= nr;

	for (c = 0; c < nr; c++)
		irq_desc[c + i].status |= IRQ_RESERVED;

	spin_unlock (&irq_alloc_lock);

	return i;
}

EXPORT_SYMBOL(alloc_irq_space);

void free_irq_space (int start, int nr)
{
	int i;

	spin_lock (&irq_alloc_lock);

	for (i = start; i < start + nr; i++) {
		irq_desc[i].handle_irq = handle_bad_irq;
		irq_desc[i].status &= ~IRQ_RESERVED;
	}

	spin_unlock (&irq_alloc_lock);
}

EXPORT_SYMBOL(free_irq_space);
