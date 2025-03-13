#ifndef HW_FFT_CORE_H
#define HW_FFT_CORE_H

	#include "qom/object.h"

	// #if defined(TARGET_AARCH64) // TODO remove all the riscv stuff
		#include "hw/arm/virt.h"
		DeviceState *fft_core_create_arm(const VirtMachineState *vms, int);
	// #else
		// DeviceState *fft_core_create_riscv(hwaddr);
	// #endif


#endif
