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
#define FFT_SIZE 0x2000


static int majorNumber;
static struct class* fftClass = NULL;
static struct device* fftDevice = NULL;
static void __iomem *fft_base;

static ssize_t fft_compute(const uint64_t *, const uint64_t *, uint64_t *, uint64_t *, size_t);
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
            fft_compute(data.input, data.inputi, data.output, data.outputi, data.len);
            break;
        default:
            return -EINVAL;
    }

    if (copy_to_user((struct fft_data *)arg, &data, sizeof(data)))
        return -EFAULT;

    return 0;
}

static ssize_t fft_compute(const uint64_t *input, const uint64_t *inputi, uint64_t *output, uint64_t *outputi, size_t len) {
    int i;

    printk(KERN_INFO "FFT: executing FFT computation with len = %d\n", len);
    
    unsigned long addr1, addr2,addr_real, addr_imag;

    for (i = 0; i < len; i++) { // TODO add a check on len and split in separate conversion if needed
        addr1 = (2*i + 0) * 8;
        addr2 = (2*i + 1) * 8;
        addr_real = (fft_base + IN_START_ID) + addr1;
        addr_imag = (fft_base + IN_START_ID) + addr2;
        
        // printk("fft_compute write64: data[%d] = 0x%X + j*0x%X, addresses: 0x%X, 0x%X\n", i, input[i], inputi[i], addr1 + IN_START_ID, addr2 + IN_START_ID);
        // printk("i = %d computes addresses %lu and %lu and value %d + j%d\n", i, addr1, addr2, input[i], inputi[i]);
        iowrite64(input[i],  addr_real);
        iowrite64(inputi[i], addr_imag);
    }
    
    iowrite32(0x1, fft_base + STATUS_ID); // Trigger FFT computation

    // Wait for computation to finish // TODO add a sleep or something
    while (ioread32(fft_base + STATUS_ID) != 0x5);
    // printk("fft_compute read32:  data = 0x%X, address: 0x%X\n", fft_base + STATUS_ID);
    
    for (i = 0; i < len; i++) {
        addr1 = (2*i + 0) * 8;
        addr2 = (2*i + 1) * 8;
        addr_real = (fft_base + OUT_START_ID) + addr1;
        addr_imag = (fft_base + OUT_START_ID) + addr2;
        
        // printk("i = %d computes addresses %lu and %lu and value %d + j%d\n", i, addr1, addr2, input[i], inputi[i]);
        output[i]  = ioread64(addr_real);
        outputi[i] = ioread64(addr_imag);
        
        // printk("fft_compute read64:  data[%d] = 0x%X + j*0x%X, addresses: 0x%X, 0x%X\n", i, output[i], outputi[i], addr1 + OUT_START_ID, addr2 + OUT_START_ID);
        // udelay(1000);
    }

    return 0;
}

static int __init fft_module_init(void) {
    printk(KERN_INFO "FFT: Initializing the FFT driver!\n");

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