#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>


MODULE_LICENSE("GPL");
MODULE_LICENSE("GPL v2");
MODULE_LICENSE("Dual BSD/GPL");

MODULE_AUTHOR("Phani");
MODULE_DESCRIPTION("A Sample driver");
MODULE_VERSION("1:0.0");

#define MAJORNO 300
#define MINORNO 0 
#define CHAR_DEV_NAME "my_cdev"
#define MAX_LENGTH 4000
#define SUCCESS 0


struct cdev *my_cdev;
dev_t mydev;
int count=1,inuse=0;

static int char_dev_open(struct inode *inode,
				struct file *file)
{
	printk(KERN_INFO "Open Operation invoked \n");
	return SUCCESS;
}

static int char_dev_release(struct inode *inode, struct file *file){
	printk(KERN_INFO "close operation invoked \n");
	return SUCCESS;
}

static ssize_t char_dev_write(struct file *file, const char __user *buf,
	size_t count, loff_t *ppos){
	printk(KERN_INFO "write operation invoked \n");
	return 0;
}

static struct file_operations char_dev_fops={
	.owner = THIS_MODULE,
	.write = char_dev_write,
	.open = char_dev_open,
	.release = char_dev_release
};

/*
int myValue,arr_myValue[4];
char *myName;
int cb_myValue=0;


module_param(myValue, int, S_IRUSR|S_IWUSR);	// integer value
module_param(myName, charp,S_IRUSR|S_IWUSR);	// String
module_param_array(arr_myValue,int,NULL,S_IRUSR|S_IWUSR); 	//Array of Integers
*/
static int __init hello_world_init(void){
	
	printk(KERN_INFO"hello world started ");
	int ret = 0;
	mydev = MKDEV(MAJORNO,MINORNO); // my dev=> 32 bit, 20Major 12Minor
	register_chrdev_region(mydev,count,CHAR_DEV_NAME);	//registering device.
	my_cdev = cdev_alloc();	// allow allocate cdev instance
	
	cdev_init(my_cdev,&char_dev_fops);  

	ret=cdev_add(my_cdev,mydev,count);// register with VFS layer , count = how many minor no. in use .
	
	if( ret < 0 ) {
		printk("Error registering device driver\n");
		return ret;
	}
	printk(KERN_INFO"\nDevice Registered %s\n",CHAR_DEV_NAME); 
 	
	return ret;
	/*
	int i;
	printk(KERN_INFO"hello world ");
	printk(KERN_INFO"myValue = %d\n",myValue);
	printk(KERN_INFO "myName = %s \n",myName);
	
	for (i=0;i<(sizeof arr_myValue / sizeof(int));i++ ){
		printk(KERN_INFO "Arr_value[%d] = %d\n",i,arr_myValue[i]);
	}
	return 0;
	*/
}


module_init(hello_world_init);

static void __exit hello_world_exit(void)
{
	cdev_del(my_cdev);
	unregister_chrdev_region(mydev,count);
	printk(KERN_INFO"Hello World exit");
}

module_exit(hello_world_exit);


