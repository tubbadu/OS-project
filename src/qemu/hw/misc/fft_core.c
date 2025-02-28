#include "qemu/osdep.h"
#include "qapi/error.h" /* provides error_fatal() handler */
#include "hw/sysbus.h" /* provides all sysbus registering func */
#include "hw/misc/fft_core.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "qemu/typedefs.h"


// Pietro's include (TODO remove this line)
#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qapi/error.h"
#include "hw/sysbus.h"
#include "qemu/typedefs.h"
#include "hw/irq.h"
#include "hw/arm/fdt.h"
#include "sysemu/device_tree.h"


#define TYPE_FFT_CORE "fft_core"
typedef struct FFTCoreState FFTCoreState;
DECLARE_INSTANCE_CHECKER(FFTCoreState, FFT_CORE, TYPE_FFT_CORE)

// static int prime(uint64_t n);
// static void compute_e(uint64_t p,uint64_t q,uint64_t t,uint64_t* e,uint64_t* d);
// static uint64_t compute_d(uint64_t t, uint64_t x);
// static uint64_t encrypt(uint64_t msg, uint64_t p, uint64_t q);
// static uint64_t decrypt(uint64_t msg, uint64_t p, uint64_t q);
// static uint64_t mod_exp(uint64_t b, uint64_t exp, uint64_t mod);




#define IN_START_ID 0x0 	//  0      // 0x4000000
#define IN_END_ID 0X318 	//  792    // 
#define OUT_START_ID 0x320	//  800    // 0x4000320
#define OUT_END_ID 0x638	//  1600   // 
#define STATUS_ID 0x640		//  1602   // 0x4000640
#define SIZE 8 // number of couple of values (real + imaginary)
#define INSIZE 32


#define PI 3.14159265358979323846

// #define N 10  // Number of values to process at a time

/*
 * 
 * 0x0 = idle
 * 0x1 = add data
 * 0x5 = finished
 * 
 * # example usage (accumulator):
 * busybox devmem 0x4000000 w 0x123  # write an input 
 * busybox devmem 0x4000640 w 0x1    # give command "plus"
 * busybox devmem 0x4000000 w 0x5000 # write another input 
 * busybox devmem 0x4000640 w 0x1    # give command "plus"
 * busybox devmem 0x4000320 64       # read output
 * busybox devmem 0x4000000 w 0x1000 # write another input 
 * busybox devmem 0x4000640 w 0x2    # give command "minus"
 * busybox devmem 0x4000320 64       # read output
 * 
 */

struct FFTCoreState {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
    uint64_t input[SIZE*2];
    uint64_t output[SIZE*2];
    uint64_t status;
};


typedef struct {
    int64_t real;
    int64_t imag;
} Complex;



static void compute_fft(FFTCoreState *, int);
// static void fft(Complex *, Complex *, int);
// static void fft_recursive(Complex *, Complex *, int, int);
// static Complex complex_multiply(Complex, Complex);
// static Complex complex_add(Complex, Complex);
// static Complex complex_subtract(Complex, Complex);
// static Complex complex_multiply(Complex, Complex);
static void fft(Complex *, Complex *, int);

/*************************************************************************************/




static void compute_fft(FFTCoreState *s, int size) {
    Complex invalues [SIZE];
    Complex outvalues[SIZE];

    for (int k=0; k<SIZE; k++){
        invalues[k].real = (int64_t) s->input[2*k    ];
        invalues[k].imag = (int64_t) s->input[2*k + 1];
    }
    
    fft(invalues, outvalues, SIZE);
    
    for (int k=0; k<SIZE; k++){
        s->output[2*k    ] = (uint64_t) outvalues[k].real;
        s->output[2*k + 1] = (uint64_t) outvalues[k].imag;
    }
    
    s->status = 0x5; // finished
}



static void fft(Complex *input, Complex *output, int n) {
    if (n <= 1) {
        output[0] = input[0];
        return;
    }
    
    Complex *evenInput = (Complex *)malloc(n / 2 * sizeof(Complex));
    Complex *oddInput = (Complex *)malloc(n / 2 * sizeof(Complex));
    Complex *evenOutput = (Complex *)malloc(n / 2 * sizeof(Complex));
    Complex *oddOutput = (Complex *)malloc(n / 2 * sizeof(Complex));
    
    for (int i = 0; i < n / 2; i++) {
        evenInput[i] = input[i * 2];
        oddInput[i] = input[i * 2 + 1];
    }
    
    fft(evenInput, evenOutput, n / 2);
    fft(oddInput, oddOutput, n / 2);
    
    for (int k = 0; k < n / 2; k++) {
        double angle = -2.0 * PI * k / n;
        Complex t = { (int64_t)(cos(angle) * oddOutput[k].real - sin(angle) * oddOutput[k].imag),
            (int64_t)(cos(angle) * oddOutput[k].imag + sin(angle) * oddOutput[k].real) };
            output[k].real = evenOutput[k].real + t.real;
            output[k].imag = evenOutput[k].imag + t.imag;
            output[k + n / 2].real = evenOutput[k].real - t.real;
            output[k + n / 2].imag = evenOutput[k].imag - t.imag;
    }
    
    free(evenInput);
    free(oddInput);
    free(evenOutput);
    free(oddOutput);
}

/*************************************************************************************/
static uint64_t fft_core_read(void *opaque, hwaddr addr, unsigned int size)
{
    FFTCoreState *s = opaque;
    
    if((int)addr >= IN_START_ID && (int)addr <= IN_END_ID && ((int)addr%8 == 0)){
        return s->input[(int)addr/8];
    } if((int)addr >= IN_START_ID && (int)addr <= IN_END_ID && ((int)addr%8 == 4)){
        return s->input[((int)addr-4)/8] >> 32;
    } else if((int)addr >= OUT_START_ID && (int)addr <= OUT_END_ID && ((int)addr%8 == 0)){
        return s->output[((int)addr-0x320)/8];
    } else if((int)addr >= OUT_START_ID && (int)addr <= OUT_END_ID && ((int)addr%8 == 4)){
        return s->output[((int)addr-0x324)/8] >> 32;
    } else if((int)addr == STATUS_ID){
        return s->status;
    } 
    
    else return 0xDEADBEEF;
    
}

static void fft_core_write(void *opaque, hwaddr addr, uint64_t data, unsigned int size)
{
    FFTCoreState *s = opaque;
    if((int)addr >= IN_START_ID && (int)addr <= IN_END_ID && ((int)addr%8 == 0)){
        s->input[(int)addr/8] = data;
        return;
    } else if((int)addr >= IN_START_ID && (int)addr <= IN_END_ID && ((int)addr%8 == 4)){
        s->input[((int)addr-4)/8] |= data << 32;
        return;
    } else if(addr == STATUS_ID){
        if(data == 0x01) {
            // new data has been written: process it
            compute_fft( s, SIZE );
            return;
        } else 
            if(data == 0x02) {
                for(int i=0; i<SIZE; i++){
                    s->output[i] -= s->input[i]; //decrypt(s->input[i], P, Q);
                }
                
                return;
            }
    } else s->status = 0x1;
}

static const MemoryRegionOps fft_core_ops = {
    .read = fft_core_read,
    .write = fft_core_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void fft_core_instance_init(Object *obj)
{
    FFTCoreState *s = FFT_CORE(obj);
    
    /* allocate memory map region */
    memory_region_init_io(&s->iomem, obj, &fft_core_ops, s, TYPE_FFT_CORE, 0x1000);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->iomem);
}

/* create a new type to define the info related to our device */
static const TypeInfo fft_core_info = {
    .name = TYPE_FFT_CORE,
    .parent = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(FFTCoreState),
    .instance_init = fft_core_instance_init,
};

static void fft_core_register_types(void)
{
    type_register_static(&fft_core_info);
}

type_init(fft_core_register_types)


// Create the fft_core device.


// #if defined(TARGET_AARCH64)
    const char compatible[] = "daem,PtrauthDevice-1.0";
    DeviceState *fft_core_create_arm(const VirtMachineState *vms, int fft_core) {
        DeviceState *dev = qdev_new(TYPE_FFT_CORE);
        MachineState *ms = MACHINE(vms);
        char *nodename;
        
        hwaddr base = vms->memmap[fft_core].base;
        hwaddr size = vms->memmap[fft_core].size;
        // int irq = vms->irqmap[fft_core];
        
        // assert(size == QARMA_REG_SIZE);
        
        sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
        sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, base);
        
        // sysbus_connect_irq(SYS_BUS_DEVICE(dev), 0, qdev_get_gpio_in(vms->gic, irq));
        
        // Register the device inside the device tree
        nodename = g_strdup_printf("/ptrauth@%" PRIx64, base);
        qemu_fdt_add_subnode(ms->fdt, nodename);
        qemu_fdt_setprop(ms->fdt, nodename, "compatible", compatible, sizeof(compatible));
        
        qemu_fdt_setprop_sized_cells(ms->fdt, nodename, "reg",
                                    2, base,
                                    2, size,
                                    2, base,
                                    2, 0);
        
        // qemu_fdt_setprop_cells(
        //     ms->fdt, nodename, "interrupts",
        //     GIC_FDT_IRQ_TYPE_SPI,      // shared periperal interrupt, can go to any core
        //     irq,
        //     GIC_FDT_IRQ_FLAGS_LEVEL_HI // high level triggered
        // );
        
        g_free(nodename);
        
        return dev;
    }
// #else
    // DeviceState *fft_core_create_riscv(hwaddr addr)
    // {
    //     DeviceState *dev = qdev_new(TYPE_FFT_CORE);
    //     sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    //     sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, addr);
    //     return dev;
    // }
// #endif

/**License: [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/)**/