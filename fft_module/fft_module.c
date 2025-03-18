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

enum FFT_State {State1 = 0x1, State2 = 0x2, State3 = 0x3, State4 = 0x4, Finished = 0x5, Error = -0x1}; // TODO find better names // TODO move to header


static size_t fft_compute(const uint64_t *, const uint64_t *, uint64_t *, uint64_t *, size_t);
// static size_t fft_compute_128(const uint64_t *, const uint64_t *, uint64_t *, uint64_t *, size_t, size_t, enum FFT_State);
static long int fft_ioctl(struct file *, unsigned int, unsigned long);

static struct file_operations fops = {
   .owner = THIS_MODULE,
   .unlocked_ioctl = fft_ioctl
};



static long int fft_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    struct fft_data data;
    

    if (copy_from_user(&data, (struct fft_data *)arg, sizeof(data)))
        return -EFAULT;
    
    if (!access_ok(data.input, sizeof(uint64_t) * data.len) ||
        !access_ok(data.inputi, sizeof(uint64_t) * data.len) ||
        !access_ok(data.output, sizeof(uint64_t) * data.len) ||
        !access_ok(data.outputi, sizeof(uint64_t) * data.len)) {
        return -EFAULT;
    }
    
    int size = sizeof(uint64_t) * data.len;
    uint64_t *input   = kmalloc(size, GFP_KERNEL);
    uint64_t *inputi  = kmalloc(size, GFP_KERNEL);
    uint64_t *output  = kmalloc(size, GFP_KERNEL);
    uint64_t *outputi = kmalloc(size, GFP_KERNEL);
    
    if (!input || !inputi || !output || !outputi) {
        kfree(input);
        kfree(inputi);
        kfree(output);
        kfree(outputi);
        return -ENOMEM;
    }
    
    
    if (copy_from_user(input, data.input, size) ||
        copy_from_user(inputi, data.inputi, size)) {
        kfree(input);
        kfree(inputi);
        kfree(output);
        kfree(outputi);
        return -EFAULT;
    }
    
    

    switch (cmd) {
        case FFT_COMPUTE:
            // fft_compute(input, inputi, output, outputi, data.len);
            for(int i=0; i<data.len; i++){
                printk("KERNEL: input[%d] = 0x%lX\n", i, input[i]);
            }
            break;
        default:
            kfree(input);
            kfree(inputi);
            kfree(output);
            kfree(outputi);
            return -EINVAL;
    }

    // Copy results back to user space
    if (copy_to_user(data.output, output, size) ||
        copy_to_user(data.outputi, outputi, size)) {
        kfree(input);
        kfree(inputi);
        kfree(output);
        kfree(outputi);
        return -EFAULT;
    }
        
    // Free kernel memory
    kfree(input);
    kfree(inputi);
    kfree(output);
    kfree(outputi);

    return 0;
}

int maxval(int a, int b){ // TODO remove if useless
    if(a > b){
        return a;
    } else {
        return b;
    }
}

int minval(int a, int b){
    if(a < b){
        return a;
    } else {
        return b;
    }
}

// enum FFT_State nextState(enum FFT_State cstate){
//     if(cstate == State1){
//         return State2;
//     } else if(cstate == State2){
//         return State3;
//     } else if(cstate == State3){
//         return State2;
//     } else {
//         return Error;
//     }
// }



// addr_offset is either 0 or 64
// io_offset is {0, 64, 128, 192, 256, 320, ...}
void fft_write(uint64_t *input, uint64_t *inputi, int addr_offset, int io_offset, int len){
    unsigned long addr1, addr2,addr_real, addr_imag;
    
    int x = len - io_offset;
    
    for (int i = 0; i < minval(64, x); i++) {
        addr1 = (2*(addr_offset + i) + 0) * 8;
        addr2 = (2*(addr_offset + i) + 1) * 8;
        addr_real = (fft_base + IN_START_ID) + addr1;
        addr_imag = (fft_base + IN_START_ID) + addr2;
        
        iowrite64(input [io_offset + i], addr_real);
        iowrite64(inputi[io_offset + i], addr_imag);
    }
    
    // zero padding
    for (int i = minval(64, x); i < 64; i++) {
        addr1 = (2*(addr_offset + i) + 0) * 8;
        addr2 = (2*(addr_offset + i) + 1) * 8;
        addr_real = (fft_base + IN_START_ID) + addr1;
        addr_imag = (fft_base + IN_START_ID) + addr2;
        
        iowrite64(0x0, addr_real);
        iowrite64(0x0, addr_imag);
    }
}

void fft_read(uint64_t *output, uint64_t *outputi, int addr_offset, int io_offset){
    unsigned long addr1, addr2,addr_real, addr_imag;
    
    for (int i = 0; i < 64; i++) {
        addr1 = (2*(addr_offset + i) + 0) * 8;
        addr2 = (2*(addr_offset + i) + 1) * 8;
        addr_real = (fft_base + OUT_START_ID) + addr1;
        addr_imag = (fft_base + OUT_START_ID) + addr2;
        printk("output: %d\n", io_offset + i);
        output [io_offset + i] = ioread64(addr_real);
        printk("outputi: %d\n", io_offset + i);
        uint64_t x = ioread64(addr_imag);
        printk("output done v1\n");
        outputi[io_offset + i] = x;
        printk("output done v2\n");
    }
}

void startFFT(uint64_t state){ // TODO rename in something like "start and wait" or "perform" or something like this
    iowrite32(state, fft_base + STATUS_ID); // Trigger FFT computation
    while (ioread32(fft_base + STATUS_ID) != Finished); // TODO add udelay
}

static size_t fft_compute(const uint64_t *input, const uint64_t *inputi, uint64_t *output, uint64_t *outputi, size_t len) {
    printk("fft_compute with len = %d    \n", len);
    
    fft_write(input, inputi, 0, 0, len); // write the first 64 values
    uint64_t cstate = State1;
    
    for(int offset = 64; offset < len; offset += 64){
        if(cstate == State1){
            fft_write(input, inputi,  64, offset, len);
            startFFT(cstate);
            fft_read(output, outputi, 0, offset);
            cstate = State2;
        } else if(cstate == State2){
            fft_write(input, inputi,  0, offset, len);
            startFFT(cstate);
            fft_read(output, outputi, 64, offset);
            cstate = State3;
        } else if(cstate == State3){
            fft_write(input, inputi,  64, offset, len);
            startFFT(cstate);
            fft_read(output, outputi, 0, offset);
            cstate = State2;
        } else {
            cstate = Error;
            break;
        }
    }
    
    return 0;
}




// static size_t fft_compute_128(const uint64_t *input, const uint64_t *inputi, uint64_t *output, uint64_t *outputi, size_t len_diff, size_t offset, enum FFT_State state) {
//     // unsigned long addr1, addr2,addr_real, addr_imag;
//     unsigned int starti, endi;
//     
//     if(state == State1){
//         starti = 0;
//         endi = 128;
//     } else if(state == State2){
//         starti = 0;
//         endi = 64;
//     } else if(state == State3){
//         starti = 64;
//         endi = 128;
//     }
// 
//     fft_write(input, inputi, starti, endi, offset);
//     
//     
//     iowrite32(state, fft_base + STATUS_ID); // Trigger FFT computation
// 
//     // Wait for computation to finish // TODO add a small sleep or something: udelay(1000);
//     while (ioread32(fft_base + STATUS_ID) != Finished); 
//     
// 
//     
//     if(state == State1 || state == State3){
//         starti = 0;
//         endi = 64;
//     } else if(state == State2){
//         starti = 64;
//         endi = 128;
//     }
// 
//     fft_read(output, outputi, starti, endi, offset);
// 
//     return 0;
// }

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