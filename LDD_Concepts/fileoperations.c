#include <linux/kernel.h>	// printk
#include <linux/fs.h>		// file open
#include <linux/device.h>  
#include <linux/init.h>        // init and exit macro
#include <linux/module.h>      // module heager
#include <linux/kdev_t.h>      // MAJOR and MINOR
#include <linux/cdev.h>        // cdev structure
#include <asm/uaccess.h>       //copy_from_user, copy_to_user

MODULE_AUTHOR("PHANI");
MODULE_LICENSE("GPL");


dev_t my_dev = 0;
static struct class *dev_class;
static struct cdev *my_cdev;


static int file_open(struct inode *node,
			struct file *file){
	printk(KERN_INFO"File open invoked successfully\n");
	pr_info("File open invoked successfully\n");			
	return 0;
}


static ssize_t file_read(struct file *file,
		             char __user *buf,
			    size_t lbuf,
			    loff_t *ppos){
	printk(KERN_INFO"File read invoked successfully\n");
	return 0;

}

/*The second argument for file_read and file_write changes --> as BUFFER varies */

static ssize_t file_write(struct file *file,
		            const char __user *buf,
			    size_t lbuf,
			    loff_t *ppos){
	printk(KERN_INFO"File write invoked successfully\n");
	return 0;

}
static int file_release(struct inode *node,
			struct file *file){
	printk(KERN_INFO"File release invoked successfully\n");	
	pr_info("File release invoked successfully\n");		
	return 0;
}

static struct file_operations fops={

	.owner = THIS_MODULE,
	.open = file_open,
	.read = file_read,
	.write = file_write,
	.release = file_release
};


static int __init file_operations_start(void){

	//Here creating a major number and minor number dynamically
	if (alloc_chrdev_region(&my_dev,0,1,"my device")<0){
		printk(KERN_INFO"Cannot allocate major and minor number for device");
		return -1;
		
	}
	
	// allocating cdev for file operations
	if(!(my_cdev = cdev_alloc())){
		printk(KERN_ERR"cdev allocation failed\n");
		return -1;
	}
	
	//creating cdev with file structure i.e. fops
	cdev_init(my_cdev,&fops);
	
	//Adding character device to the system
	if((cdev_add(my_cdev,my_dev,1))<0){
		printk(KERN_ERR"Failed to register device driver for file operatins\n");
		cdev_del(my_cdev);
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


static void __exit file_operations_end(void){
	
	device_destroy(dev_class,my_dev);
	class_destroy(dev_class);
	//unregistering the device
	cdev_del(my_cdev);
	unregister_chrdev_region(my_dev,1);
	printk(KERN_INFO"Kernel Module Removed..\n");
}


module_init(file_operations_start);
module_exit(file_operations_end);
