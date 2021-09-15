#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
//s#include<sys/loctl.h>

#define MAX_SIZE 1024

int main()
{
	int fd=open("/dev/my_cdev",O_RDWR);// we created this using "sudo mknode /dev/my_chardev c 300 0"
					        // can check this using "ls-al /dev "
	return 0;
	close(fd);
}
