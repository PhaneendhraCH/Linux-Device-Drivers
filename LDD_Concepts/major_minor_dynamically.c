#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

MODULE_AUTHOR("PHANI");
MODULE_LICENSE("GPL");

dev_t my_dev = 0; // assigning major and minor statically

static int __init major_minor_start(void){

	//Here creating a major number and minor number dynamically
	if (alloc_chrdev_region(&my_dev,0,1,"my device")<0){
		printk(KERN_INFO"Cannot allocate major and minor number for device");
		return -1;
		
	}
	
	//register_chrdev_region(my_dev,1,"my device");
	printk(KERN_INFO"Major = %d Minor = %d\nKernel Module Inserted Successfully\n",MAJOR(my_dev),MINOR(my_dev));
	return 0;
}


static void __exit major_minor_end(void){
	
	//unregistering the device
	unregister_chrdev_region(my_dev,1);
	printk(KERN_INFO"Kernel Module Removed..\n");
}


module_init(major_minor_start);
module_exit(major_minor_end);
