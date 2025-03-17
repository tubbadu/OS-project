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
#include <complex.h>

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
#include "../../../fftlib/fft_algorithm.h"


#define TYPE_FFT_CORE "fft_core"
typedef struct FFTCoreState FFTCoreState;
DECLARE_INSTANCE_CHECKER(FFTCoreState, FFT_CORE, TYPE_FFT_CORE)

// static int prime(uint64_t n);
// static void compute_e(uint64_t p,uint64_t q,uint64_t t,uint64_t* e,uint64_t* d);
// static uint64_t compute_d(uint64_t t, uint64_t x);
// static uint64_t encrypt(uint64_t msg, uint64_t p, uint64_t q);
// static uint64_t decrypt(uint64_t msg, uint64_t p, uint64_t q);
// static uint64_t mod_exp(uint64_t b, uint64_t exp, uint64_t mod);



#define STATUS_ID 0x0
#define IN_START_ID 0x8
#define IN_END_ID 0x808
#define OUT_START_ID 0x810
#define OUT_END_ID 0x1010

#define floatoint *(uint64_t*)&
#define inttofloat *(double*)&

#define SIZE 128 // number of couple of values (real + imaginary)
#define INSIZE 32


#define PI 3.14159265358979323846 // TODO change this with the constant inside math.h

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

enum FFT_State {State1 = 0x1, State2 = 0x2, State3 = 0x3, State4 = 0x4, Finished = 0x5}; // TODO find better names // TODO move to header




static void compute_fft(FFTCoreState *, int, enum FFT_State);
static void compute_fft_128(FFTCoreState *, enum FFT_State);

/*************************************************************************************/




static void compute_fft_128(FFTCoreState *s, enum FFT_State state) {
    double complex val [SIZE];
    

    if(state == State1 || state == State3){
        for (int k=0; k<SIZE; k++){
            val[k] = (inttofloat s->input[2*k]) + I * (inttofloat s->input[2*k + 1]);
        }
    } else if(state == State2){
        // swap the two halves of the input vector
        for (int k=0; k<SIZE/2; k++){
            val[k + SIZE/2] = (inttofloat s->input[2*k]) + I * (inttofloat s->input[2*k + 1]);
        }
        for (int k=SIZE/2; k<SIZE; k++){
            val[k - SIZE/2] = (inttofloat s->input[2*k]) + I * (inttofloat s->input[2*k + 1]);
        }
    } else {
        // ERROR! TODO add
        qemu_log("ERROR #1!\n");
        return;
    }
    
    
    FFT(val, SIZE, 1);
    // now val contains the results of this specific 128 values fft
    
    if(state == State1){
        // overwrite the output
        for (int k=0; k<SIZE; k++){
            double a, b;
            a = creal(val[k]);
            b = cimag(val[k]);
            
            s->output[2*k    ] = (floatoint a);
            s->output[2*k + 1] = (floatoint b);
        }
    } else if(state == State2){
        // overwrite second half of val in the first half of output
        for (int k=0; k<SIZE/2; k++){
            double a, b;
            a = creal(val[k + SIZE/2]);
            b = cimag(val[k + SIZE/2]);
            
            s->output[2*k    ] = (floatoint a);
            s->output[2*k + 1] = (floatoint b);
        }
        // add first half of val in the second half of output
        for (int k=SIZE/2; k<SIZE; k++){
            double a, b;
            a = creal(val[k - SIZE/2]) + (inttofloat s->output[2*k    ]);
            b = cimag(val[k - SIZE/2]) + (inttofloat s->output[2*k + 1]);
            
            s->output[2*k    ] = (floatoint a);
            s->output[2*k + 1] = (floatoint b);
        }
    } else if(state == State3){
        // add first half of val in the first half of output
        for (int k=0; k<SIZE/2; k++){
            double a, b;
            a = creal(val[k]) + (inttofloat s->output[2*k    ]);
            b = cimag(val[k]) + (inttofloat s->output[2*k + 1]);
            
            s->output[2*k    ] = (floatoint a);
            s->output[2*k + 1] = (floatoint b);
        }
        // overwrite second half of val in the second half of output
        for (int k=SIZE/2; k<SIZE; k++){
            double a, b;
            a = creal(val[k]);
            b = cimag(val[k]);
            
            s->output[2*k    ] = (floatoint a);
            s->output[2*k + 1] = (floatoint b);
        }
    } else {
        // ERROR! TODO add
        qemu_log("ERROR #2!\n");
        return;
    }

    s->status = Finished; // finished
}



static void compute_fft(FFTCoreState *s, int size, enum FFT_State state) {
    return compute_fft_128(s, state);
}



/*************************************************************************************/
static uint64_t fft_core_read(void *opaque, hwaddr addr, unsigned int size)
{
    FFTCoreState *s = opaque;

    int addri = (int)addr;
    uint64_t ret;
    
    // qemu_log("addri = %d, reminder = %d, size = %d\n", addri, addri % 8, size);
    
    if(addri == STATUS_ID){
        ret = s->status;
    } else if(IN_START_ID <= addri && addri <= IN_END_ID){
        if(addri % 8 == 0){
            ret = s->input[ (addri - IN_START_ID) / 8 ];
            // qemu_log("read: i = %d\n", (addri - IN_START_ID)/8);
        } else if(addri % 8 == 4){
            ret = s->input[ (addri - IN_START_ID - 4) / 8 ] >> 32;
            // qemu_log("read: i = %d, >> 32\n", (addri - IN_START_ID - 4)/8);
        } else ret = 0xDEADBEEF;
    } else if(OUT_START_ID <= addri && addri <= OUT_END_ID){
        if(addri % 8 == 0){
            ret = s->output[ (addri - OUT_START_ID) / 8 ];
            // qemu_log("read: i = %d\n", addri/8);
        } else if(addri % 8 == 4){
            ret = s->output[ (addri - OUT_START_ID - 4) / 8 ] >> 32;
            // qemu_log("read: i = %d, >> 32\n", (addri - OUT_START_ID - 4)/8);
        } else ret = 0xDEADBEEF;
    } else ret = 0xDEADBEEF;
    
    // qemu_log("READ:  address = 0x%X, data = 0x%X, size = %d\n", addri, ret, size);
    return ret;
}

static void fft_core_write(void *opaque, hwaddr addr, uint64_t data, unsigned int size)
{
    FFTCoreState *s = opaque;
    int addri = (int)addr;
    
    // qemu_log("WRITE: address = 0x%X, data = 0x%X, size = %d\n", addri, data, size);
    
    if(addri == STATUS_ID){
        if(0x1 <= data && data <= 0x3){ // if it's an existing operation
            // start the conversion
            compute_fft( s, SIZE, data );
            return;
        } else {
            s->status = 0xF; // 0xF = error
        }
    } else if(IN_START_ID <= addri && addri <= IN_END_ID){
        if(addri % 8 == 0){
            s->input[ (addri - IN_START_ID) / 8 ] = data;
            // qemu_log("write: i = %d\n", (addri - IN_START_ID)/8);
        } else if(addri % 8 == 4){
            s->input[ (addri - IN_START_ID - 4) / 8 ] |= data << 32;
            // qemu_log("write: i = %d, << 32\n", (addri - IN_START_ID - 4)/8);
        }  
    } else {
        s->status = 0xF; // 0xF = error
        qemu_log("WRITE ERROR!\n");
    }
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
    memory_region_init_io(&s->iomem, obj, &fft_core_ops, s, TYPE_FFT_CORE, 0x2000); // TODO change with a define
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