//#include <linux/kernel.h>
//#include <linux/module.h>
//#include <linux/init.h>
//#include <linux/fs.h>

//#include <linux/kdev_t.h>
//#include <linux/cdev.h>
//#include <linux/device.h>
//#include <linux/fcntl.h> /*Helps fix O_ACCMODE*/
//#include <linux/sched.h> /*Helps fix TASK_UNINTERRUPTIBLE */
//#include <linux/fs.h> /*Helps fix the struct intializer */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>        /* kmalloc() */
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

#include <linux/uaccess.h>

#include <linux/io.h>
#include <linux/version.h>
#include <linux/completion.h>   // Header File for Completation
#include <asm/current.h>

/*
 Completion Structure :

    struct completion {
        unsigned int done;              // done, like acts as FLAG
        struct swait_queue_head wait;   // wait queue
    };

 */

dev_t dev =0;


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Phani");
MODULE_DESCRIPTION("Completation in Linux");
MODULE_VERSION("1.0.0");

static struct class *dev_class;     // dynamic inode
static struct cdev my_cdev;         // cddev strucutre to intreact with VFS Layer
uint8_t *kernel_buffer = NULL;      // to perform read and write

#define MEM_SIZE        1024           //Memory Size

// Function Prototyes
static int      __init my_driver_init(void);
static void     __exit my_driver_exit(void);
static int      my_open(struct inode *inode, struct file *file);
static int      my_release(struct inode *inode, struct file *file);
static ssize_t  my_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  my_write(struct file *filp, const char *buf, size_t len, loff_t * off);

//*************************COMPLETATION**********************************

/*

 Actual Definition of the macro :

 #define DECLARE_COMPLETION(work) \
	struct completion work = COMPLETION_INITIALIZER(work)

 */

DECLARE_COMPLETION(my_wait); // declaring using Macro

//***********************************************************************

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = my_read,
    .write      = my_write,
    .open       = my_open,
    .release    = my_release,
};


static int my_open(struct inode *inode, struct file *file)
{
	
	if((kernel_buffer = kmalloc(MEM_SIZE,GFP_KERNEL))==0)
	{
		printk(KERN_INFO "Cannot allocate memory in kernel\n");
		return -1;
	}
	printk(KERN_INFO"Driver open  function called .....\n");
	return 0;
}


static int my_release(struct inode *inode, struct file *file)
{
	kfree(kernel_buffer);
	printk(KERN_INFO"Driver release function called ....\n");
	return 0;
}


static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	int ret = 0;
	
    /*

     static inline void reinit_completion(struct completion *x)
        {
            x->done = 0;
        }

    Description :

        This inline function should be used to reinitialize a completion structure so it can
        be reused. This is especially important after complete_all() is used.

     */

	reinit_completion (&my_wait);//resets the ->done field to 0 (“not done”), without touching the waitqueue
	
	printk (KERN_INFO "process %i (%s) going to sleep\n", current->pid,current->comm);
	wait_for_completion (&my_wait);
    printk (KERN_INFO "process %i (%s) resuming\n", current->pid,current->comm);

	
	//Copy the data from the kernel space to the user-space
    ret = copy_to_user(buf, kernel_buffer, MEM_SIZE);
	if(ret > 0)
	{
		printk(KERN_INFO"writing data to user-space failed\n");
	}
	printk(KERN_INFO"Driver read function called .....\n");
	return 0;	
	
}


static ssize_t my_write(struct file *filp, const char __user *buf,size_t len,loff_t *off)
{
	int ret = 0;
	//Copy the data to kernel space from the user-space
	copy_from_user(kernel_buffer, buf, len);
	if(ret > 0)
	{
		printk(KERN_INFO"Copy the data to kernel space from the user-space\n");
	}
	printk(KERN_INFO"Driver write function called .%s... \n",kernel_buffer);
	
	printk (KERN_INFO "process %i (%s) wake up readers...\n",current->pid, current->comm);
    complete (&my_wait);        // assigns the value to 0.
	
	return len;
} 


static int __init my_driver_init(void)
{
	if((alloc_chrdev_region(&dev,0,1,"my_Char"))<0)
	{
		printk(KERN_INFO"cannot allocate major and minor number");
		return -1;
	}
	printk(KERN_INFO"Major =%d Minor = %d \n",MAJOR(dev),MINOR(dev));
	
	//creating cdev structure
	cdev_init(&my_cdev,&fops);
	
	//Adding charriver to system
	if((cdev_add(&my_cdev,dev,1)) <0)
	{
		printk(KERN_INFO"cannot add the device to the system \n");
		goto r_class;
	}

	if((dev_class = class_create(THIS_MODULE,"my_Char_class")) == NULL){
		printk(KERN_INFO"Cannot create the struct class for device");
		goto r_class;
	}
	if((device_create(dev_class,NULL,dev,NULL,"my_Char_driver"))==NULL) {
		printk(KERN_INFO" cannot create the device\n");
		goto r_device;
	}

	printk(KERN_INFO"character drivre init sucess\n");

	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}


static void __exit my_driver_exit(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO "kernel driver removed  ... done \n");

}
module_init(my_driver_init);
module_exit(my_driver_exit);
