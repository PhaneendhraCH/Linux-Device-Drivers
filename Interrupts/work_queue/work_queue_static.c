#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>


#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fcntl.h> 		/*Helps fix O_ACCMODE*/
#include <linux/sched.h> 		/*Helps fix TASK_UNINTERRUPTIBLE */
#include <linux/fs.h> 			/*Helps fix the struct intializer */

#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>

#include <linux/proc_fs.h>    	// proc file system
#include <linux/compiler.h>
#include <linux/types.h>
#include <linux/proc_ns.h>

#include <linux/kobject.h>

#include <linux/interrupt.h>

#include <linux/kthread.h> 		// therad 
#include <linux/delay.h>   		// sleep 
#include <linux/sched.h>               //task_struct

#include <linux/timer.h>
#include <linux/jiffies.h>

#include <linux/workqueue.h> 		// Required for workqueues

#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <asm/uaccess.h>


#define SHARED_IRQ 19	// Interrupt Line 
static int irq = SHARED_IRQ, my_dev_id, irq_counter = 0; // declaration 
module_param (irq, int, S_IRUGO);	// S_IRUGO for a parameter that can be read by the world but cannot be changed;


void workq_fn(struct work_struct *work); //function prototype

DECLARE_WORK(workq,workq_fn); 	//macro for work queue


void workq_fn(struct work_struct *work)
{
	
	//long c;
	
	
	printk(KERN_INFO"my work queue static function");
	
	if(in_interrupt())
        printk(KERN_INFO "Running as an interrupt");
	else
		 printk(KERN_INFO "Running as an process context");
		 
	atomic_long_set(&(workq.data),10);	
	printk(KERN_INFO "In workq function %u",atomic_long_read(&(workq.data)));
	
	
}


//Interrupt Handler Function
static irqreturn_t my_interrupt (int irq, void *dev_id)
{

    /*
    
    Here the work queue runs on the process context
    
    */

    printk(KERN_INFO"ISR has Started");
    schedule_work(&workq);	// scheduling work_queue function
    printk(KERN_INFO"ISR has Stopped");
    return IRQ_NONE;            /* we return IRQ_NONE because we are just observing */
}


//init function
static int __init my_workqueue_init(void)
{
	if (request_irq(irq, my_interrupt, IRQF_SHARED, "my_interrupt_user", &my_dev_id))
        return -1;

	printk(KERN_INFO"My workqueue init static function");
return 0;
}


//exit function
static void __exit my_workqueue_exit(void)
{
	flush_scheduled_work();
	
	free_irq (irq, &my_dev_id); //Remove an interrupt handler. The handler is removed and if the interrupt line is no longer in use by any driver it is disabled. On a shared IRQ the caller must ensure the interrupt is disabled on the card it drives before calling this function.
	printk(KERN_INFO"My workqueue exit  static function");
}

module_init(my_workqueue_init);
module_exit(my_workqueue_exit);


MODULE_AUTHOR ("Phani");
MODULE_DESCRIPTION ("Static Work Queue");
MODULE_LICENSE ("GPL");


