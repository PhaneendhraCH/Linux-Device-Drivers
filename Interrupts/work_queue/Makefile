obj-m :=work_queue_dynamic.o
#obj-m :=work_queue_static.o

KDIR= /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules
	
clean:
	make -C $(KDIR) M=$(shell pwd) clean
