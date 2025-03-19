#ifndef HW_FFT_CORE_H
#define HW_FFT_CORE_H
	#include "qom/object.h"
	#include "hw/arm/virt.h"
	DeviceState *fft_core_create_arm(const VirtMachineState *vms, int);
#endif
