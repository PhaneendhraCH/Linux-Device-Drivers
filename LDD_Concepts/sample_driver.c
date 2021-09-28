#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

MODULE_AUTHOR("Phani");
MODULE_LICENSE("GPL");


static int __init hello_world_start(void){

	printk("Driver Inserted..\n");
	return 0;
}


static void __exit hello_world_end(void){

	printk("Driver Exited..\n");
}

module_init(hello_world_start);
module_exit(hello_world_end);
