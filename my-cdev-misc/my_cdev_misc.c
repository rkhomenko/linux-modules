// SPDX-License-Identifier: GPL-2.0-or-later

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Khomenko <romankhomenko1995@gmail.com>");
MODULE_VERSION("1.0");

static char *device_data = "This is data from device\n";
static int device_open;

static int minor;
module_param(minor, int, 0444);

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

static const struct file_operations misc_fops = {
	.owner = THIS_MODULE,
	.open = my_cdev_open,
	.release = my_cdev_release,
	.read = my_cdev_read
};

static struct miscdevice misc_dev = {
	MISC_DYNAMIC_MINOR,
	"my_cdev_misc",
	&misc_fops
};

static int __init my_cdev_init(void)
{
	int ret = 0;

	if (minor != 0) 
		misc_dev.minor = minor;
	ret = misc_register(&misc_dev);

	if (ret) {
		pr_err("=== Can not register misc device\n");
		goto err;
	}

	pr_info("========= module installed %d =========\n", misc_dev.minor);

err:
	return ret;
}

static void __exit my_cdev_exit(void)
{
	misc_deregister(&misc_dev);
	pr_info("========= module removed =========\n");
}

module_init(my_cdev_init);
module_exit(my_cdev_exit);

