# QEMU & Buildroot Setup Guide

This guide provides step-by-step instructions to set up QEMU and Buildroot on Ubuntu, modify necessary files, and use the provided Makefile for automation.

## 1. Install Required Packages
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

## 2. Install QEMU
Clone the QEMU repository and check out the desired version:

```sh
git clone https://github.com/qemu/qemu.git
cd qemu
git checkout v6.0.0
```

Initialize and update QEMU submodules:

```sh
git submodule init
git submodule update
```

Create a build directory and configure QEMU for ARM64 emulation:

```sh
mkdir build
cd build
../configure --target-list=aarch64-softmmu
make -j$(nproc)
```

## 3. Install Buildroot
Clone the Buildroot repository:

```sh
git clone https://github.com/buildroot/buildroot.git
cd buildroot/
```

Configure Buildroot for QEMU ARM64:

```sh
make qemu_aarch64_virt_defconfig
make -j$(nproc)
```

## 4. Modify Necessary Files
Use the provided Python script to copy and replace required files:

```sh
python3 scripts/sync-files.py
```

This script iterates through the relevant directories in QEMU and Buildroot, ensuring the necessary modifications are applied.

## 5. Run QEMU with Buildroot
Navigate to the generated images directory and start QEMU:

```sh
cd output/images
sh start-qemu.sh
```

This will launch QEMU with the configured Buildroot system.

---

# FFT_CORE

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

### Usage Examples

#### Build and Run QEMU for RISC-V
```sh
make build-qemu-riscv
make run-qemu-riscv
```

#### Build and Run QEMU for ARM
```sh
make build-qemu-arm
make run-qemu-arm
```

#### Modify and Sync Necessary Files
```sh
make sync-files
```

#### Build Buildroot for ARM
```sh
make buildroot-build-arm
```

#### Create Setup for ARM
```sh
make create-setup-arm
```

#### Build and Run Everything for ARM
```sh
make all-arm
```

#### Build and Run Everything for RISC-V
```sh
make all-riscv
```

This Makefile, along with the provided Python script, ensures that QEMU and Buildroot are properly set up, required files are modified, and both ARM and RISC-V architectures can be built and tested efficiently.

