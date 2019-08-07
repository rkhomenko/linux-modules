KERNEL_VERSION = $(shell uname -r)
KERNEL_DIR = /lib/modules/$(KERNEL_VERSION)/build
TARGET = template_module

CLFAGS_template_module.o := -std=gnu89 -Wall -Wextra -pedantic

obj-m := $(TARGET).o
default:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	@rm -f *.o *.mod.c *.order

distclean: clean
	@rm *.ko *.symvers
