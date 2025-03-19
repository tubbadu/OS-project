# QEMU: FFT Core Integration

This document details the modifications made to the QEMU source files to integrate a custom hardware component: the FFT Core.

The FFT Core supports 128 input values and 128 output values, each 64 bits long, and operates on `double complex` numbers.

## Custom Hardware

The file [fft_core.c](https://github.com/tubbadu/OS-project/blob/main/src/qemu/hw/misc/fft_core.c) defines the FFT Core hardware. It specifies the memory region boundaries (input, output, and status) and includes a data structure for exchanging data with the kernel module.

To read and write values to memory-mapped registers, the functions `fft_core_read` and `fft_core_write` are implemented. They allow reading and writing chunks of data aligned to 8 or 4 bytes, applying masking and shifting when necessary.

Computation begins when the start code (`0x1`) is written to the status register, triggering the `compute_fft` function. Once completed, the output registers store the results.

The function `fft_core_create_arm` initializes and registers the custom FFT Core device in the QEMU ARM virtual machine. It allocates the device, maps it to memory, registers it on the system bus, and updates the device tree with its properties.

## QEMU Memory Mapping

To register and map the FFT Core in the QEMU virtual machine, the file [virt.c](https://github.com/tubbadu/OS-project/blob/main/src/qemu/hw/arm/virt.c) has been modified accordingly.

### Memory Map Entry

```c
static const MemMapEntry base_memmap[] = {
    ...
    [VIRT_SECURE_GPIO] =        { 0x090b0000, 0x00001000 },
    [VIRT_FFT_CORE] =           { 0x090c0000, 0x00002000 }, 
    [VIRT_MMIO] =               { 0x0a000000, 0x00000200 },
    ...
};

```

### Device initialization
```c
    ...
    create_uart(vms, VIRT_UART, sysmem, serial_hd(0));
    
    /* fft_core device */
    fft_core_create_arm(vms, VIRT_FFT_CORE);

    if (vms->secure) {
        create_secure_ram(vms, secure_sysmem, secure_tag_sysmem);
    ...
```

The header file [virt.h](https://github.com/tubbadu/OS-project/blob/main/src/qemu/include/hw/arm/virt.h) has been modified to add `VIRT_FFT_CORE` inside the enumeration. It has been added as the second-to-last value in the list, as the last space is reserved for `VIRT_LOWMEMMAP_LAST`. 

Other minor modifications and additional files can be found in the [repository](https://github.com/tubbadu/OS-project/tree/main/src/qemu).







