// SPDX-License-Identifier: GPL-2.0-or-later

#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Khomenko <romankhomenko1995@gmail.com>");

static int __init module_init_proc(void)
{
	pr_info("Template module start!\n");
	return 0;
}

static void __exit module_exit_proc(void)
{
	pr_info("Template module exited!\n");
}

module_init(module_init_proc);
module_exit(module_exit_proc);

