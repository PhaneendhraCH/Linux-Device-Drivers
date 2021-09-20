obj-m+= atomic_variable_incandtest.o
#obj-m+= atomic_variable_decandtest.o
#obj-m+= atomic_variable_add.o
#obj-m+= atomic_variable_sub.o
#obj-m+= atomic_variable_decrement.o

KDIR = /lib/modules/$(shell uname -r)/build 

all :
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
