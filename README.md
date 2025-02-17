# QEMU & Buildroot Setup Guide

This guide provides step-by-step instructions to set up QEMU and Buildroot on Ubuntu, modify necessary files, and use the provided Makefile for automation.

## Install Required Packages
To ensure a smooth setup, install the necessary dependencies using the following command:

```sh
sudo apt update && sudo apt install -y git libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev \
  ninja-build build-essential python3 python3-pip libaio-dev libcap-ng-dev \
  libiscsi-dev libattr1-dev libnfs-dev libudev-dev libxen-dev libepoxy-dev \
  libdrm-dev libgbm-dev libvirglrenderer-dev libgtk-3-dev libspice-protocol-dev \
  libspice-server-dev libusb-1.0-0-dev libpulse-dev libsdl2-dev libslirp-dev \
  libncurses5-dev libncursesw5-dev libx11-dev libxext-dev libxt-dev \
  libpng-dev libjpeg8-dev libvte-2.91-dev libfuse-dev git make rsync
```

# Makefile Guide for QEMU and Buildroot (ARM & RISC-V)

This repository provides a Makefile to automate the process of building and running QEMU and Buildroot for both ARM and RISC-V architectures.

## Available Makefile Targets

Run `make help` to see available targets:

```sh
make help
```

### Makefile Targets

- `build-qemu-riscv` - Configure and build QEMU for RISC-V.
- `run-qemu-riscv` - Run QEMU for RISC-V.
- `build-qemu-arm` - Configure and build QEMU for ARM.
- `run-qemu-arm` - Run QEMU for ARM.
- `sync-files` - Synchronize QEMU modified files.
- `all-riscv` - Sync, build, and run QEMU for RISC-V.
- `all-arm` - Sync, build, and run QEMU for ARM.
- `all` - Sync, build, and run QEMU for ARM.
- `create-setup-arm` - Create initial setup for ARM, cloning both QEMU and Buildroot.
- `buildroot-build-arm` - Build Buildroot for ARM.


## Build Instructions:

1. Clone this repository:

   ```shell
   git clone https://github.com/tubbadu/OS-project/tree/main
   cd OS-project
   ```

2. Create `.env` file:

   ```shell
   make env
   ```

3. Clone [Buildroot](https://github.com/buildroot/buildroot) and [QEMU](https://github.com/qemu/qemu) repositories (if you already cloned them, you can move them inside `OS-project` and skip this step):

   ```shell
   make clone-repos
   ```

4. Create the setup for the build environment:

   ```shell
   make create-setup-arm
   ```

5. Build Buildroot (it will take a while):

   ```shell
   make buildroot-build-arm
   ```

6. Insert the FFT_CORE source files in QEMU:

   ```shell
   make sync-files
   ```

7. Build QEMU with the FFT_CORE (it will take a while):

   ```shell
   make build-qemu-arm
   ```

8. Run the modified QEMU:

   ```shell
   make run-qemu-arm
   ```

---

This Makefile, along with the provided Python script, ensures that QEMU and Buildroot are properly set up, required files are modified, and both ARM and RISC-V architectures can be built and tested efficiently.

