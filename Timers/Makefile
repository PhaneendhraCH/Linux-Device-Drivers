obj-m +=timer_jiffie_count.o
#obj-m +=ktimer_test.o

KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
