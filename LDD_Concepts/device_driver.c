#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>

MODULE_AUTHOR("PHANI");
MODULE_LICENSE("GPL");


dev_t my_dev = 0;
static struct class *dev_class;

static int __init major_minor_start(void){

	//Here creating a major number and minor number dynamically
	if (alloc_chrdev_region(&my_dev,0,1,"my device")<0){
		printk(KERN_INFO"Cannot allocate major and minor number for device");
		return -1;
		
	}
	/* Creating struct class */
	if ((dev_class = class_create(THIS_MODULE,"class1"))== NULL){
	
		printk(KERN_INFO"Failed to create the class");
		unregister_chrdev_region(my_dev,1);	
		return -1;
	}
	
	/* Creating device*/
	if((device_create(dev_class,NULL,my_dev,NULL,"%s","my_char_driver"))== NULL){
		printk(KERN_INFO "Unable to create the device class for struct class");
		class_destroy(dev_class);
		
	}
	
	//register_chrdev_region(my_dev,1,"my device");
	printk(KERN_INFO"Major = %d Minor = %d\nKernel Module Inserted Successfully\n",MAJOR(my_dev),MINOR(my_dev));
	return 0;
}


static void __exit major_minor_end(void){
	
	
	device_destroy(dev_class,my_dev);
	class_destroy(dev_class);
	//unregistering the device
	unregister_chrdev_region(my_dev,1);
	printk(KERN_INFO"Kernel Module Removed..\n");
}


module_init(major_minor_start);
module_exit(major_minor_end);
