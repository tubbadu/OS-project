# OS Project - FFT Core

This project is a Linux kernel module designed to perform Fast Fourier Transform (FFT). It is then used on an audio signal in order to determine the corresponding musical note.

By integrating this functionality directly into the kernel, the module enables efficient real-time frequency analysis, making it suitable for applications in audio signal processing, musical analysis, and embedded systems. The implementation is optimized for execution in an emulated environment using QEMU and Buildroot, ensuring flexibility and portability across different hardware platforms.


- For environment setup instructions, refer to the [Setup.md](./Setup.md) file.
- Detailed explanations of the kernel module, see [KernelModule.md](./KernelModule.md).
- To learn more about the modifications made to QEMU for custom HW emulation, check [Qemu.md](./Qemu.md).
- The example application is described in [ExampleApplication.md](./ExampleApplication.md).

---

You can find the complete repository on [GitHub](https://github.com/tubbadu/OS-project).