#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#define SHARED_IRQ 19	// Interrupt Line 
static int irq = SHARED_IRQ, my_dev_id, irq_counter = 0; // declaration 
module_param (irq, int, S_IRUGO);	// S_IRUGO for a parameter that can be read by the world but cannot be changed;

static irqreturn_t my_interrupt (int irq, void *dev_id)
{
    irq_counter++;
    printk (KERN_INFO "In the ISR: counter = %d\n", irq_counter);
    return IRQ_NONE;            /* we return IRQ_NONE because we are just observing */
}

static int __init my_init (void)
{
    if (request_irq(irq, my_interrupt, IRQF_SHARED, "my_interrupt", &my_dev_id))
        return -1;
    printk (KERN_INFO "Successfully loading ISR handler\n");
    return 0;
}

static void __exit my_exit (void)
{
    synchronize_irq (irq);
    free_irq (irq, &my_dev_id);
    printk (KERN_INFO "Successfully unloading \n");
}

module_init (my_init);
module_exit (my_exit);

MODULE_AUTHOR ("Phani");
MODULE_DESCRIPTION ("Interrupt Handler Test");
MODULE_LICENSE ("GPL");
