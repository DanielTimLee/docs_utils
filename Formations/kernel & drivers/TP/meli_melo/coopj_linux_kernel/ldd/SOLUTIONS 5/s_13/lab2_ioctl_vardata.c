/* **************** CLDD:1.0 s_13/lab2_ioctl_vardata.c **************** */
/*
 * The code herein is: Copyright 1998-2009 by Jerry Cooperstein
 *
 * This Copyright is retained for the purpose of protecting free
 * redistribution of source.
 *
 * URL:    http://www.coopj.com
 * email:  coop@coopj.com 
 *
 * The primary maintainer for this code is Jerry Cooperstein
 * The CONTRIBUTORS file (distributed with this
 * file) lists those known to have contributed to the source.
 *
 * This code is distributed under Version 2 of the GNU General Public
 * License, which you should have received with the source.
 *
 */

/*
 * Using ioctl's to pass data of variable length.
 *
 * Extend the previous exercise to send a buffer whose length is
 * determined at run time.  You will probably need to use the _IOC
 * macro directly in the user-space program.  (See linux/ioctl.h.)
 @*/

#include <linux/module.h>
#include "lab_char.h"

#define MYIOC_TYPE 'k'

static inline long
mycdrv_unlocked_ioctl (struct file *fp, unsigned int cmd, unsigned long arg)
{
    int i, rc, direction;
    int size;
    char *buffer;
    void __user *ioargp = (void __user *)arg;

    /* make sure it is a valid command */

    if (_IOC_TYPE (cmd) != MYIOC_TYPE) {
        printk (KERN_WARNING " got invalid case, CMD=%d\n", cmd);
        return -EINVAL;
    }

    /* get the size of the buffer and kmalloc it */

    size = _IOC_SIZE (cmd);
    buffer = kmalloc ((size_t) size, GFP_KERNEL);
    if (!buffer) {
        printk (KERN_ERR "Kmalloc failed for buffer\n");
        return -ENOMEM;
    }

    /* fill it with X */

    memset (buffer, 'X', size);

    direction = _IOC_DIR (cmd);

    switch (direction) {

    case _IOC_WRITE:
        printk
            (KERN_INFO
             " reading = %d bytes from user-space and writing to device\n",
             size);
        rc = copy_from_user (buffer, ioargp, size);
        printk (KERN_INFO "rc from copy_from_user = %d\n", rc);
        break;

    case _IOC_READ:
        printk (KERN_INFO
                " reading device and writing = %d bytes to user-space\n", size);
        rc = copy_to_user (ioargp, buffer, size);
        printk (KERN_INFO "rc from copy_to_user = %d\n", rc);
        break;

    default:
        printk (KERN_WARNING " got invalid case, CMD=%d\n", cmd);
        return -EINVAL;
    }
    for (i = 0; i < size; i++)
        printk (KERN_INFO "%c", buffer[i]);
    printk (KERN_INFO "\n");

    if (buffer)
        kfree (buffer);
    return rc;
}

static const struct file_operations mycdrv_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = mycdrv_unlocked_ioctl,
    .open = mycdrv_generic_open,
    .release = mycdrv_generic_release
};

module_init (my_generic_init);
module_exit (my_generic_exit);

MODULE_AUTHOR ("Jerry Cooperstein");
MODULE_DESCRIPTION ("CLDD:1.0 s_13/lab2_ioctl_vardata.c");
MODULE_LICENSE ("GPL v2");
