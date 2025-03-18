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

enum FFT_State {State1 = 0x1, State2 = 0x2, State3 = 0x3, State4 = 0x4, Finished = 0x5}; // TODO find better names // TODO move to header


static size_t fft_compute(const uint64_t *, const uint64_t *, uint64_t *, uint64_t *, size_t);
static size_t fft_compute_128(const uint64_t *, const uint64_t *, uint64_t *, uint64_t *, size_t, size_t, enum FFT_State);
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


// TODO write functions to write and read in defined intervals!

void fft_write(const uint64_t *input, const uint64_t *inputi, int starti, int endi, int offset){
    unsigned long addr1, addr2,addr_real, addr_imag;
    
    for (int i = starti; i < endi; i++) {
        addr1 = (2*i + 0) * 8;
        addr2 = (2*i + 1) * 8;
        addr_real = (fft_base + IN_START_ID) + addr1;
        addr_imag = (fft_base + IN_START_ID) + addr2;
        printk("w1    \n");
        iowrite64(input [offset + i], addr_real);
        printk("w2    \n");
        iowrite64(inputi[offset + i], addr_imag);
    }
}

void fft_read(uint64_t *output, uint64_t *outputi, int starti, int endi, int offset){
    unsigned long addr1, addr2,addr_real, addr_imag;
    printk("starti = %d, endi = %d, offset = %d    \n", starti, endi, offset);
    for (int i = starti; i < endi; i++) {
        printk("i = %d    \n", i);
        addr1 = (2*i + 0) * 8;
        addr2 = (2*i + 1) * 8;
        addr_real = (fft_base + OUT_START_ID) + addr1;
        addr_imag = (fft_base + OUT_START_ID) + addr2;
        printk("r1    \n");
        output [offset + i] = ioread64(addr_real);
        printk("r2: %d    \n", offset + i);
        // outputi[offset + i] = 5;
        // printk("r2.1    \n");
        // int pippo = ioread64(addr_imag);
        // printk("r2.2    \n");
        outputi[offset + i] = ioread64(addr_imag);
    }
}

static size_t fft_compute(const uint64_t *input, const uint64_t *inputi, uint64_t *output, uint64_t *outputi, size_t len) {
    int i = 0;
    printk("fft_compute with len = %d    \n", len);
    fft_compute_128(input, inputi, output, outputi, (len-i), i, State1);
    
    i += 128; // because State1 writes 128 values
    if(i >= len){
        // read last
        printk("2    \n");
        fft_read(output, outputi, 64, 128, 0);
    } else {
        while(1){
            printk("3    \n");
            fft_compute_128(input, inputi, output, outputi, (len-i), i, State2);
            i += 64;
            if(i >= len){
                // read last TODO check indexes! in this specific case
                printk("4    \n");
                fft_read(output, outputi, 0, (len - (i-64)), (i-64));
                break;
            }
            printk("5    \n");
            fft_compute_128(input, inputi, output, outputi, (len-i), i, State3);
            i += 64;
            if(i >= len){
                // read last TODO check indexes! in this specific case
                printk("6    \n");
                fft_read(output, outputi, 0, (len - (i-64)), (i-64));
                break;
            }
        }
    }
    printk("7    \n");
    return 0;
}




static size_t fft_compute_128(const uint64_t *input, const uint64_t *inputi, uint64_t *output, uint64_t *outputi, size_t len_diff, size_t offset, enum FFT_State state) {
    // unsigned long addr1, addr2,addr_real, addr_imag;
    unsigned int starti, endi;
    
    if(state == State1){
        starti = 0;
        endi = 128;
    } else if(state == State2){
        starti = 0;
        endi = 64;
    } else if(state == State3){
        starti = 64;
        endi = 128;
    }

    fft_write(input, inputi, starti, endi, offset);
    
    
    iowrite32(state, fft_base + STATUS_ID); // Trigger FFT computation

    // Wait for computation to finish // TODO add a small sleep or something: udelay(1000);
    while (ioread32(fft_base + STATUS_ID) != Finished); 
    

    
    if(state == State1 || state == State3){
        starti = 0;
        endi = 64;
    } else if(state == State2){
        starti = 64;
        endi = 128;
    }

    fft_read(output, outputi, starti, endi, offset);

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