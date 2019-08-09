KERNEL_VERSION = $(shell uname -r)
KERNEL_DIR = /lib/modules/$(KERNEL_VERSION)/build
TARGET = template_module

CFLAGS_template_module.o := -std=gnu89 -Wstrict-prototypes -Wmissing-prototypes

obj-m := $(TARGET).o
default:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order
	@rm -f .*.*.cmd
	@rm -fR .tmp*
	@rm -rf .tmp_versions

distclean: clean
	@rm -f *.ko *.symvers
