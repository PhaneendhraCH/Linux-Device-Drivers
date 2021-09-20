#include <linux/kernel.h>	// kernel header
#include <linux/kthread.h>	// kthread header
#include <linux/delay.h>	// sleep 
#include <linux/mutex.h>	// mutex header
#include <linux/module.h>

MODULE_AUTHOR("Phani");
MODULE_LICENSE("GPL");

static atomic_t my_global_var  = ATOMIC_INIT(10); // declaring and initialsing to 0
static struct task_struct *char_thread;
static struct task_struct *char_thread1;


int my_thread1(void *p){
	while(!(kthread_should_stop())){
	atomic_dec(&my_global_var);
	printk(KERN_INFO"my_thread1 invoked and my_global_val after dec %d\n",atomic_read(&my_global_var));
	msleep(1000);
	}
	return 0;
}

int my_thread2(void *p){
	while(!(kthread_should_stop())){
	atomic_dec(&my_global_var);
	printk(KERN_INFO"my_thread2 invoked and my_global_val after dec %d\n",atomic_read(&my_global_var));
	msleep(1000);
	}
	return 0;
}

static int __init thread_func_start(void){

	printk(KERN_INFO"Sample thread driver started..\n");

	char_thread = kthread_run(my_thread1,NULL,"my char thread");
	if(char_thread<0)
		printk(KERN_INFO"Unable to create and wake up thread\n");
	else
		printk(KERN_INFO"Thread Created and running\n");
		
	char_thread1 = kthread_run(my_thread2,NULL,"my char thread1");
	if(char_thread1<0)
		printk(KERN_INFO"Unable to create and wake up thread\n");
	else
		printk(KERN_INFO"Thread Created and running\n");
	return 0;
}


static void __exit thread_func_end(void){
	kthread_stop(char_thread);
	kthread_stop(char_thread1);
	printk(KERN_INFO"Sample thread driver exited..\n");
}


module_init(thread_func_start);
module_exit(thread_func_end);

