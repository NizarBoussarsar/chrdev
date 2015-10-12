obj-m := cutbuf.o
KDIR = /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	cp *.ko /lib/modules/$(shell uname -r)/kernel
	depmod -a

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

ins:
	@insmod ./cutbuf.ko
	@echo "Loading module messages in /var/log/messages:"
	@dmesg

probe:
	@cp cutbuf /etc/modprobe.d/
	@modprobe cutbuf
	@echo "Loading module messages in /var/log/messages:"
	@dmesg

rm:
	@rmmod cutbuf
	@echo "Unloading module messages in /var/log/messages:"
	@dmesg
