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

#define TYPE_FFT_CORE "fft_core"
typedef struct FFTCoreState FFTCoreState;
DECLARE_INSTANCE_CHECKER(FFTCoreState, FFT_CORE, TYPE_FFT_CORE)

// static int prime(uint64_t n);
// static void compute_e(uint64_t p,uint64_t q,uint64_t t,uint64_t* e,uint64_t* d);
// static uint64_t compute_d(uint64_t t, uint64_t x);
// static uint64_t encrypt(uint64_t msg, uint64_t p, uint64_t q);
// static uint64_t decrypt(uint64_t msg, uint64_t p, uint64_t q);
// static uint64_t mod_exp(uint64_t b, uint64_t exp, uint64_t mod);

#define IN_START_ID 0x0 	//  0      // 
#define IN_END_ID 0X318 	//  792    // 
#define OUT_START_ID 0x320	//  800    // 0x4000320
#define OUT_END_ID 0x638	//  1600   // 
#define STATUS_ID 0x640		//  1602   // 0x4000640
#define SIZE 100
#define P 89
#define Q 181

/*
 
example usage:
busybox devmem 0x4000000 w 0x123 # write an input 
busybox devmem 0x4000640 w 0x1   # give command "plus 1"
busybox devmem 0x4000320 64      # read output
busybox devmem 0x4000640 w 0x2   # give command "minus 1"
busybox devmem 0x4000320 64      # read output
 
*/

struct FFTCoreState {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
    uint64_t input[SIZE];
    uint64_t output[SIZE];
    uint64_t status;
};


// void fft(Complex input[], Complex output[]) {
//     int N = FFT_SIZE;
//     
//     // Copy input to output
//     for (int i = 0; i < N; i++) {
//         output[i].real = input[i].real;
//         output[i].imag = input[i].imag;
//     }
//     
//     // Bit-reversal permutation (simplified for small N)
//     int j = 0;
//     for (int i = 1; i < N - 1; i++) {
//         int bit = N / 2;
//         while (j >= bit) {
//             j -= bit;
//             bit /= 2;
//         }
//         j += bit;
//         if (i < j) {
//             Complex temp = output[i];
//             output[i] = output[j];
//             output[j] = temp;
//         }
//     }
//     
//     // Compute the FFT
//     for (int len = 2; len <= N; len *= 2) {
//         double angle = -2.0 * M_PI / len;
//         Complex wLen = {cos(angle), sin(angle)};
//         
//         for (int i = 0; i < N; i += len) {
//             Complex w = {1.0, 0.0};
//             for (int j = 0; j < len / 2; j++) {
//                 Complex even = output[i + j];
//                 Complex odd = output[i + j + len / 2];
//                 
//                 Complex temp;
//                 temp.real = w.real * odd.real - w.imag * odd.imag;
//                 temp.imag = w.real * odd.imag + w.imag * odd.real;
//                 
//                 output[i + j].real = even.real + temp.real;
//                 output[i + j].imag = even.imag + temp.imag;
//                 output[i + j + len / 2].real = even.real - temp.real;
//                 output[i + j + len / 2].imag = even.imag - temp.imag;
//                 
//                 // Update w
//                 double newReal = w.real * wLen.real - w.imag * wLen.imag;
//                 double newImag = w.real * wLen.imag + w.imag * wLen.real;
//                 w.real = newReal;
//                 w.imag = newImag;
//             }
//         }
//     }
// }

static uint64_t pippo_read(void *opaque, hwaddr addr, unsigned int size)
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

static void pippo_write(void *opaque, hwaddr addr, uint64_t data, unsigned int size)
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
            for(int i=0; i<SIZE; i++){
                s->output[i] = s->input[i] + 1; //encrypt(s->input[i], P, Q);
            }
            
            return;
        } else 
            if(data == 0x02) {
                for(int i=0; i<SIZE; i++){
                    s->output[i] = s->input[i] - 1; //decrypt(s->input[i], P, Q);
                }
                
                return;
            }
    } else s->status = 0x1;
}

static const MemoryRegionOps fft_core_ops = {
    .read = pippo_read,
    .write = pippo_write,
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

/*
 * Create the FFT_core device.
 */
DeviceState *fft_core_create(hwaddr addr)
{
    DeviceState *dev = qdev_new(TYPE_FFT_CORE);
    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, addr);
    return dev;
}

/**License: [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/)**/
