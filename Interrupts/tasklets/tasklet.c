
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>


#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fcntl.h> /*Helps fix O_ACCMODE*/
#include <linux/sched.h> /*Helps fix TASK_UNINTERRUPTIBLE */
#include <linux/fs.h> /*Helps fix the struct intializer */

#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>

#include <linux/proc_fs.h>    // proc file system
#include <linux/compiler.h>
#include <linux/types.h>
#include <linux/proc_ns.h>

#include <linux/kobject.h>

#include <linux/interrupt.h>

#include <linux/kthread.h> // therad 
#include <linux/delay.h>   // sleep 
#include <linux/sched.h>               //task_struct

#include <linux/timer.h>
#include <linux/jiffies.h>

#include <linux/workqueue.h> // Required for workqueues

#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <asm/uaccess.h>


static void my_tasklet_handler(struct tasklet_struct *my_task); //prototype for tasklet handler

#define SHARED_IRQ 19	// Interrupt Line 
static int irq = SHARED_IRQ, my_dev_id, irq_counter = 0; // declaration 
module_param (irq, int, S_IRUGO);	// S_IRUGO for a parameter that can be read by the world but cannot be changed;


// Name : my_tasklet, Function : my_tasklet_handler, data :0
DECLARE_TASKLET(my_tasklet,my_tasklet_handler); 



// Tasklet Handler Function
static void my_tasklet_handler(struct tasklet_struct *my_task)
{
	//tasklet_disable(&my_tasklet);
	printk(KERN_INFO"\nMy tasklet handler function");
	//tasklet_enable(&my_tasklet);
	irq_counter++;
    	printk (KERN_INFO "In the ISR: counter = %d\n", irq_counter);
}


static irqreturn_t my_interrupt (int irq, void *dev_id)
{

    tasklet_schedule(&my_tasklet);	// scheduling tasklet function
    return IRQ_NONE;            /* we return IRQ_NONE because we are just observing */
}

static int __init my_init (void)
{
    if (request_irq(irq, my_interrupt, IRQF_SHARED, "my_interrupt_user", &my_dev_id))
        return -1;
    printk (KERN_INFO "Successfully loading ISR handler\n");
    return 0;
}

static void __exit my_exit (void)
{
    tasklet_kill(&my_tasklet); // Terminate the Tasklet
    
    printk(KERN_INFO"My tasklet function exit");
    
    synchronize_irq (irq); //This function waits for any pending IRQ handlers for this interrupt to complete before returning. If you use this function while holding a resource the IRQ handler may need you will deadlock.
    
    free_irq (irq, &my_dev_id); //Remove an interrupt handler. The handler is removed and if the interrupt line is no longer in use by any driver it is disabled. On a shared IRQ the caller must ensure the interrupt is disabled on the card it drives before calling this function.
    
    printk (KERN_INFO "Successfully unloading \n");
}

module_init (my_init);
module_exit (my_exit);

MODULE_AUTHOR ("Phani");
MODULE_DESCRIPTION ("Interrupt Handler Test");
MODULE_LICENSE ("GPL");
