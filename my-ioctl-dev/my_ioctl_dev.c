// SPDX-License-Identifier: GPL-2.0-or-later

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#include "ioctl.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Khomenko <romankhomenko1995@gmail.com>");
MODULE_VERSION("1.0");

static char *device_data = "This is data from device\n";

enum {
	EOK = 0
};

static int my_cdev_open(struct inode *n, struct file *f)
{
	return -EOK;
}

static int my_cdev_release(struct inode *n, struct file *f)
{
	return -EOK;
}

static ssize_t my_cdev_read(struct file *file,
			    char *buffer,
			    size_t count,
			    loff_t *ppos)
{
	size_t len = strlen(device_data);

	pr_info("=== read: %lu\n", count);

	if (count < len)
		return -EINVAL;

	if (*ppos != 0) {
		pr_info("=== read return : 0\n");
		return 0;
	}

	if (copy_to_user(buffer, device_data, len))
		return -EINVAL;

	*ppos = len;
	return len;
}

static long dev_ioctl(struct file *f, unsigned int cmd,
			  unsigned long arg)
{
	if (_IOC_TYPE(cmd) != IOC_MAGIC)
		return -ENOTTY;

	switch (cmd) {
	case IOCTL_GET_STRING:
		if (copy_to_user((void *)arg, device_data,
				 strlen(device_data)))
			return -EFAULT;
		break;
	default:
		return -ENOTTY;
	}

	return -EOK;
}

static const struct file_operations dev_fops = {
	.owner = THIS_MODULE,
	.open = my_cdev_open,
	.release = my_cdev_release,
	.read = my_cdev_read,
	.unlocked_ioctl = dev_ioctl
};

static struct class *dev_class;

#define DEVICE_NAME "my_ioctl_dev"
#define DEVICE_FIRST 0
#define CLASS_NAME "my_ioctl_dev"

static int major;

static int __init my_cdev_init(void)
{
	dev_t dev;

	major = register_chrdev(0, DEVICE_NAME, &dev_fops);
	if (major < 0) {
		pr_err("=== Can not register char device\n");
		goto err;
	}


	dev_class = class_create(THIS_MODULE, CLASS_NAME);
	dev = MKDEV(major, DEVICE_FIRST);
	device_create(dev_class, NULL, dev, "%s", "my_ioct_dev_0");

	pr_info("========= module installed %d =========\n", major);

err:
	return 0;
}

static void __exit my_cdev_exit(void)
{
	dev_t dev;

	dev = MKDEV(major, DEVICE_FIRST);
	device_destroy(dev_class, dev);
	class_destroy(dev_class);

	unregister_chrdev(major, DEVICE_NAME);

	pr_info("========= module removed =========\n");
}

module_init(my_cdev_init);
module_exit(my_cdev_exit);

