obj-m+= chr_drv_dynamic.o
#obj-m is to build as module
#obj-y is to build as statically

#it refers the kernel build scripts form below path and build the driver

KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules

clean:
	make -C $(KDIR) M=$(shell pwd) clean
	
