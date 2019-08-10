/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef MY_IOCTL_DEV_IOCTL_H_
#define MY_IOCTL_DEV_IOCTL_H_

struct return_string {
	char buf[160];
};

#define IOC_MAGIC 'h'
#define IOCTL_GET_STRING _IOR(IOC_MAGIC, 1, struct return_string)
#define DEVICE_PATH "/dev/my_ioctl_dev"

#endif /* MY_IOCTL_DEV_IOCTL_H_ */
