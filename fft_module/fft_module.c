#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include "fft_module.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group 06");
MODULE_DESCRIPTION("A driver for the FFT hardware accelerator");
MODULE_VERSION("1.0");

#define DEVICE_NAME "fft"
#define CLASS_NAME "fft_class"

#define FFT_BASE_ADDR 0x090c0000
#define FFT_SIZE 0x1000

#define FFT_INPUT_OFFSET 0x0
#define FFT_OUTPUT_OFFSET 0x320
#define FFT_STATUS_OFFSET 0x640

static int majorNumber;
static struct class* fftClass = NULL;
static struct device* fftDevice = NULL;
static void __iomem *fft_base;

static ssize_t fft_compute(const uint64_t *, uint64_t *, size_t);
static long int fft_ioctl(struct file *, unsigned int, unsigned long);

static struct file_operations fops = {
   .owner = THIS_MODULE,
   .unlocked_ioctl = fft_ioctl
};

static long int fft_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    struct fft_data data;

    if (copy_from_user(&data, (struct fft_data *)arg, sizeof(data)))
        return -EFAULT;

    switch (cmd) {
        case FFT_COMPUTE:
            fft_compute(data.input, data.output, data.len);
            break;
        default:
            return -EINVAL;
    }

    if (copy_to_user((struct fft_data *)arg, &data, sizeof(data)))
        return -EFAULT;

    return 0;
}

static ssize_t fft_compute(const uint64_t *input, uint64_t *output, size_t len) {
    int i;

    printk(KERN_INFO "FFT: executing FFT computation\n");

    for (i = 0; i < len; i++) {
        iowrite64(input[i], fft_base + FFT_INPUT_OFFSET + i * 8);
    }

    iowrite32(0x1, fft_base + FFT_STATUS_OFFSET); // Trigger FFT computation

    // Wait for computation to finish
    while (ioread32(fft_base + FFT_STATUS_OFFSET) != 0x5);

    for (i = 0; i < len; i++) {
        output[i] = ioread64(fft_base + FFT_OUTPUT_OFFSET + i * 8);
    }

    return 0;
}

static int __init fft_module_init(void) {
    printk(KERN_INFO "FFT: Initializing the FFT driver!!!!???\n");

    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        printk(KERN_ALERT "FFT failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "FFT: registered correctly with major number %d\n", majorNumber);

    fftClass = class_create(CLASS_NAME);
    if (IS_ERR(fftClass)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(fftClass);
    }
    printk(KERN_INFO "FFT: device class registered correctly\n");

    fftDevice = device_create(fftClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(fftDevice)) {
        class_destroy(fftClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(fftDevice);
    }
    printk(KERN_INFO "FFT: device class created correctly\n");

    fft_base = ioremap(FFT_BASE_ADDR, FFT_SIZE);
    if (!fft_base) {
        device_destroy(fftClass, MKDEV(majorNumber, 0));
        class_destroy(fftClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "FFT: Failed to map memory\n");
        return -ENOMEM;
    }

    return 0;
}

static void __exit fft_module_exit(void) {
    iounmap(fft_base);
    device_destroy(fftClass, MKDEV(majorNumber, 0));
    class_unregister(fftClass);
    class_destroy(fftClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "FFT: Goodbye from the FFT driver!!!\n");
}

module_init(fft_module_init);
module_exit(fft_module_exit);