/* **************** LDD:1.0 s_14/lab1_firmware.c **************** */
/*
 * The code herein is: Copyright Jerry Cooperstein and the Linux Foundation, 2009
 *
 * This Copyright is retained for the purpose of protecting free
 * redistribution of source.
 *
 *     URL:    http://www.coopj.com
 *     email:  coop@linuxfoundation.org
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
 * Loading Firmware

 * Write a module that loads some firmware from the filesystem. It
 * should print out the contents.
 *    
 * In order to do this you'll need to place a firmware file under
 * /lib/firmware. You can use just a text file for the purpose of this
 * demonstration.
    
 * Since this is a pseudo device, you will have to declare and
 * initialize a device structure. Minimally you must set the void
 * (*release)(struct device *dev) field in this structure, and call
 *     
 *    int dev_set_name (struct device *dev, const char *fmt, ...);
 *    
 * to set the device name, which can be read out with:
 *    
 *    const char *dev_name(const struct device *dev);
 *    
 * (You may want to see what happens if you neglect setting one of
 * these quantities.)
 *        
 * Make sure you call
 *          
 *    device_register  (struct device *dev);
 *    device_unregister(struct device *dev);
 *    
 * before requesting and after releasing the firmware.
 *    
 @*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/firmware.h>

#define FWFILE "my_fwfile"
static const struct firmware *fw;

static void my_release (struct device *dev)
{
    printk (KERN_INFO "releasing firmware device\n");
}

static struct device dev = {
    .release = my_release
};

static int __init my_init (void)
{
    dev_set_name (&dev, "my0");
    device_register (&dev);
    printk (KERN_INFO "firmware_example: my device inserted\n");

    if (request_firmware (&fw, FWFILE, &dev)) {
        printk (KERN_ERR "requesting firmware failed\n");
        device_unregister (&dev);
        return -1;
    }
    printk (KERN_INFO "firmware contents=\n%s\n", fw->data);
    return 0;
}
static void __exit my_exit (void)
{
    release_firmware (fw);
    device_unregister (&dev);
    printk (KERN_INFO "release firwmare and unregistered device\n");
}

module_init (my_init);
module_exit (my_exit);

MODULE_LICENSE ("GPL");
