obj-m += my_timer.o

KDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KKDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean