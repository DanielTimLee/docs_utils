/* **************** LDD:1.0 s_04/sample_driver.c **************** */
/*
 * The code herein is: Copyright Jerry Cooperstein, 2009
 *
 * This Copyright is retained for the purpose of protecting free
 * redistribution of source.
 *
 *     URL:    http://www.coopj.com
 *     email:  coop@coopj.com
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
Sample Character Driver 
@*/

#include <linux/module.h>       /* for modules */
#include <linux/fs.h>           /* file_operations */
#include <linux/uaccess.h>      /* copy_(to,from)_user */
#include <linux/init.h>         /* module_init, module_exit */
#include <linux/slab.h>         /* kmalloc */
#include <linux/cdev.h>         /* cdev utilities */

#define MYDEV_NAME "mycdrv"
#define KBUF_SIZE (size_t) (10*PAGE_SIZE)

static char *kbuf;
static dev_t first;
static unsigned int count = 1;
static int my_major = 700, my_minor = 0;
static struct cdev *my_cdev;

static int mycdrv_open (struct inode *inode, struct file *file)
{
    printk (KERN_INFO " OPENING device: %s:\n\n", MYDEV_NAME);
    return 0;
}

static int mycdrv_release (struct inode *inode, struct file *file)
{
    printk (KERN_INFO " CLOSING device: %s:\n\n", MYDEV_NAME);
    return 0;
}

static ssize_t
mycdrv_read (struct file *file, char __user * buf, size_t lbuf, loff_t * ppos)
{
    int nbytes = lbuf - copy_to_user (buf, kbuf + *ppos, lbuf);
    *ppos += nbytes;
    printk (KERN_INFO "\n READING function, nbytes=%d, pos=%d\n", nbytes,
            (int)*ppos);
    return nbytes;
}
static ssize_t
mycdrv_write (struct file *file, const char __user * buf, size_t lbuf,
              loff_t * ppos)
{
    int nbytes = lbuf - copy_from_user (kbuf + *ppos, buf, lbuf);
    *ppos += nbytes;
    printk (KERN_INFO "\n WRITING function, nbytes=%d, pos=%d\n", nbytes,
            (int)*ppos);
    return nbytes;
}
static const struct file_operations mycdrv_fops = {
    .owner = THIS_MODULE,
    .read = mycdrv_read,
    .write = mycdrv_write,
    .open = mycdrv_open,
    .release = mycdrv_release,
};
static int __init my_init (void)
{
    kbuf = kmalloc (KBUF_SIZE, GFP_KERNEL);
    first = MKDEV (my_major, my_minor);
    register_chrdev_region (first, count, MYDEV_NAME);
    my_cdev = cdev_alloc ();
    cdev_init (my_cdev, &mycdrv_fops);
    cdev_add (my_cdev, first, count);
    printk (KERN_INFO "\nSucceeded in registering character device %s\n",
            MYDEV_NAME);
    return 0;
}
static void __exit my_exit (void)
{
    cdev_del (my_cdev);
    unregister_chrdev_region (first, count);
    printk (KERN_INFO "\ndevice unregistered\n");
    kfree (kbuf);
}

module_init (my_init);
module_exit (my_exit);

MODULE_AUTHOR ("Jerry Cooperstein");
MODULE_LICENSE ("GPL v2");
