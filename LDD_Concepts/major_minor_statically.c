#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

MODULE_AUTHOR("PHANI");
MODULE_LICENSE("GPL");

dev_t my_dev = MKDEV(235,0); // assigning major and minor statically

static int __init major_minor_start(void){

	register_chrdev_region(my_dev,1,"my device");
	printk(KERN_INFO"Major = %d Minor = %d\nKernel Module Inserted Successfully\n",MAJOR(my_dev),MINOR(my_dev));
	return 0;
}


static void __exit major_minor_end(void){
	
	unregister_chrdev_region(my_dev,1);
	printk(KERN_INFO"Kernel Module Removed..\n");
}


module_init(major_minor_start);
module_exit(major_minor_end);
