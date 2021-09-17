#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include<sys/ioctl.h>
#include <errno.h> // used to print the error number values
#define MY_MAGIC 'a'
#define WR_VALUE _IOW(MY_MAGIC,'a',int32_t*)
#define RD_VALUE _IOR(MY_MAGIC,'b',int32_t*)
#define MY_IOCTL_MAX_CMD 2

int main()
{
	int fd;
    int32_t value, number;	
	
	printf("[%d] - Opening device my_cdrv\n", getpid() );
	
	fd = open( "/dev/ts_device", O_RDWR );
	
	fd = open( "/dev/andkand", O_RDWR );
	
	if( fd < 0 ) {
		printf("\n\nDevice could not be opened\n\n");
		printf("Error No. %d and type %s\n",errno,strerror(errno));
		return 1;
	}
	printf("Device opened with ID [%d]\n", fd);
	
	   
	printf("Enter the Value to send\n");
	scanf("%d",&number);
	printf("Writing Value to Driver\n");
	
	/*
	Returns 0 or non-negative value on succcess and 
	Returns -1 for error and errno is set appropriately.
	*/
	
	if(ioctl(fd, WR_VALUE, (int32_t*) &number)<0){
		printf("Error %d type %s\n",errno,strerror(errno));
		return errno;
	} 

	printf("Reading Value from Driver\n");
	
	if(ioctl(fd, RD_VALUE, (int32_t*) &value)<0){
		printf("Error : %d type %s",errno,strerror(errno));
		return errno;
	}
	printf("Value is %d\n", value);

	printf("Closing Driver\n");
	
	if(close(fd)<0){
		printf("Error : %d type %s",errno,strerror(errno));
	}
	
	
	exit(0);
}


