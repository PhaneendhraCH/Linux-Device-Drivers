#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include<unistd.h>
int main(){

int fd;

fd = open("/dev/my_char_driver",O_RDWR);
if (fd<0)
	printf("Unable to readfile\n");
else
	printf("File Read Successfully\n");
	
close(fd);
return 0;

}

