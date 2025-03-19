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


#define STATUS_ID 0x0
#define IN_START_ID 0x8
#define IN_END_ID 0x808
#define OUT_START_ID 0x810
#define OUT_END_ID 0x1010

#define floatoint *(uint64_t*)&
#define inttofloat *(double*)&

#define SIZE 128 // number of couple of values (real + imaginary)


struct FFTCoreState {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
    uint64_t input[SIZE*2];
    uint64_t output[SIZE*2];
    uint64_t status;
};


static void compute_fft(FFTCoreState *, int);

/*************************************************************************************/




static void compute_fft(FFTCoreState *s, int size) {
    double complex val [SIZE];
    
    for (int k=0; k<SIZE; k++){
        val[k] = (inttofloat s->input[2*k]) + I * (inttofloat s->input[2*k + 1]);
    }
    
    FFT(val, SIZE, 1); // Why SIZE*2 is needed?
    
    
    for (int k=0; k<SIZE; k++){
        double a, b;
        a = creal(val[k]);
        b = cimag(val[k]);
        
        s->output[2*k    ] = (floatoint a);
        s->output[2*k + 1] = (floatoint b);
    }
    s->status = 0x5; // finished
}





/*************************************************************************************/
static uint64_t fft_core_read(void *opaque, hwaddr addr, unsigned int size)
{
    FFTCoreState *s = opaque;

    int addri = (int)addr;
    uint64_t ret;
        
    if(addri == STATUS_ID){
        ret = s->status;
    } else if(IN_START_ID <= addri && addri <= IN_END_ID){
        if(addri % 8 == 0){
            ret = s->input[ (addri - IN_START_ID) / 8 ];
        } else if(addri % 8 == 4){
            ret = s->input[ (addri - IN_START_ID - 4) / 8 ] >> 32;
        } else ret = 0xDEADBEEF;
    } else if(OUT_START_ID <= addri && addri <= OUT_END_ID){
        if(addri % 8 == 0){
            ret = s->output[ (addri - OUT_START_ID) / 8 ];
        } else if(addri % 8 == 4){
            ret = s->output[ (addri - OUT_START_ID - 4) / 8 ] >> 32;
        } else ret = 0xDEADBEEF;
    } else ret = 0xDEADBEEF;
    
    return ret;
}

static void fft_core_write(void *opaque, hwaddr addr, uint64_t data, unsigned int size)
{
    FFTCoreState *s = opaque;
    int addri = (int)addr;
    
    
    if(addri == STATUS_ID){
        if(data == 0x1){
            // start the conversion
            compute_fft( s, SIZE );
            return;
        } else {
            s->status = 0xF;
        }
    } else if(IN_START_ID <= addri && addri <= IN_END_ID){
        if(addri % 8 == 0){
            s->input[ (addri - IN_START_ID) / 8 ] = data;
        } else if(addri % 8 == 4){
            s->input[ (addri - IN_START_ID - 4) / 8 ] |= data << 32;
        }  
    } else {
        s->status = 0xF;
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
    memory_region_init_io(&s->iomem, obj, &fft_core_ops, s, TYPE_FFT_CORE, 0x2000);
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
const char compatible[] = "daem,PtrauthDevice-1.0";
DeviceState *fft_core_create_arm(const VirtMachineState *vms, int fft_core) {
    DeviceState *dev = qdev_new(TYPE_FFT_CORE);
    MachineState *ms = MACHINE(vms);
    char *nodename;
    
    hwaddr base = vms->memmap[fft_core].base;
    hwaddr size = vms->memmap[fft_core].size;
        
    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, base);
    
    nodename = g_strdup_printf("/ptrauth@%" PRIx64, base);
    qemu_fdt_add_subnode(ms->fdt, nodename);
    qemu_fdt_setprop(ms->fdt, nodename, "compatible", compatible, sizeof(compatible));
    
    qemu_fdt_setprop_sized_cells(ms->fdt, nodename, "reg",
                                2, base,
                                2, size,
                                2, base,
                                2, 0);

    g_free(nodename);
    
    return dev;
}
