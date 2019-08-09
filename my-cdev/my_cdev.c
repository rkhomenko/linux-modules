// SPDX-License-Identifier: GPL-2.0-or-later

#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Khomenko <romankhomenko1995@gmail.com>");
MODULE_VERSION("1.0");

static char *device_data = "This is data from device\n";
static int device_open;

static int major;
module_param(major, int, 0444);

enum {
	EOK = 0
};

static int my_cdev_open(struct inode *n, struct file *f)
{
	if (device_open)
		return -EBUSY;
	device_open++;
	return -EOK;
}

static int my_cdev_release(struct inode *n, struct file *f)
{
	device_open--;
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

static const struct file_operations dev_fops = {
	.owner = THIS_MODULE,
	.open = my_cdev_open,
	.release = my_cdev_release,
	.read = my_cdev_read
};

#define DEVICE_FIRST 0
#define DEVICE_COUNT 3
#define MODNAME "my_cdev"

static struct cdev hcdev;

static int __init my_cdev_init(void)
{
	int ret = 0;
	dev_t dev;

	if (major != 0) {
		dev = MKDEV(major, DEVICE_FIRST);
		ret = register_chrdev_region(dev, DEVICE_COUNT, MODNAME);
	} else {
		ret = alloc_chrdev_region(&dev, DEVICE_FIRST,
					  DEVICE_COUNT, MODNAME);
		major = MAJOR(dev);
	}

	if (ret < 0) {
		pr_err("=== Can not register char device region\n");
		goto err;
	}

	cdev_init(&hcdev, &dev_fops);
	hcdev.owner = THIS_MODULE;

	ret = cdev_add(&hcdev, dev, DEVICE_COUNT);
	if (ret < 0) {
		unregister_chrdev_region(MKDEV(major, DEVICE_FIRST),
					 DEVICE_COUNT);
		pr_err("=== Can not add char device\n");
		goto err;
	}

	pr_info("========= module installed %d:%d =========\n",
		MAJOR(dev), MINOR(dev));

err:
	return ret;
}

static void __exit my_cdev_exit(void)
{
	cdev_del(&hcdev);
	unregister_chrdev_region(MKDEV(major, DEVICE_FIRST),
				 DEVICE_COUNT);
	pr_info("========= module removed =========\n");
}

module_init(my_cdev_init);
module_exit(my_cdev_exit);

