obj-m:= Procfs_1.o
#obj-m+= mutex_kernel.o
#obj-m+= threadskernel.o
#obj-m+= fileoperations.o
#obj-m+= device_driver.o
#obj-m+= major_minor_dynamically.o
#obj-m+= sample_driver.o

KDIR = /lib/modules/$(shell uname -r)/build 

all :
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
