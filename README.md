# QEMU & Buildroot Setup Guide

This guide provides step-by-step instructions to set up QEMU and Buildroot on Ubuntu, modify necessary files, and use the provided Makefile for automation.

## Install Required Packages

To ensure a smooth setup, install the necessary dependencies using the following command:

```sh
sudo apt update && sudo apt install -y $(cat requirements.txt)  
```

# Makefile Guide for QEMU and Buildroot (ARM & RISC-V)

This repository provides a Makefile to automate the process of building and running QEMU and Buildroot for both ARM and RISC-V architectures.

## Available Makefile Targets

Run `make help` to see available targets:

```sh
make help
```

### Makefile Targets

- `env`                       - Create .env file.
- `clone-repos`               - Clone QEMU and Buildroot repositories.
- `create-setup-arm`          - Create initial setup for ARM, cloning both QEMU and Buildroot.
- `build-buildroot-arm`       - Build Buildroot for ARM.
- `build-qemu-arm`            - Configure and build QEMU for ARM.
- `run-qemu-arm`              - Run QEMU for ARM.
- `apply-mods`                - Sync modified files.
- `build-kernel-module-arm`   - Build kernel module for ARM.
- `build-test`                - Build kernel module test.
- `copy-test`                 - Copy test executable to root filesystem.

## Build Instructions:

1. Clone this repository:
   
  ```shell
  git clone https://github.com/tubbadu/OS-project.git
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

4. Create the setup for the build environment: (perhaps unnecessary? TODO)
   
  ```shell
  make create-setup-arm
  ```

5. Build Buildroot (it will take a while):
   
  ```shell
  make buildroot-build-arm
  ```

6. Apply the modifications to the source files of QEMU and Buildroot:
   
  ```shell
  make apply-mods
  ```

7. Build QEMU with the FFT_CORE (it will take a while):
   
  ```shell
  make build-qemu-arm
  ```
   
8. Build the kernel module inside Buildroot:

  ```shell
  make build-kernel-module-arm
  ```

9. Cross-compile the test:

  ```shell
  make build-test
  ```
10. Copy the test inside the root filesystem of the buildroot image

  ```shell
  make copy-test
  ```
  
11. Run the Buildroot image with the modified QEMU:
   
  ```shell
  make run-qemu-arm
  ```
  
When prompted for a login, type `root` and press Enter. Congrats, you are now inside the VM!
  
You can install the kernel module with:
```shell
/opt/install-kernel-module.sh
```

You can then run the test:
```shell
/opt/fft_test
```
  
  
---

This Makefile, along with the provided Python script, ensures that QEMU and Buildroot are properly set up, required files are modified, and both ARM and RISC-V architectures can be built and tested efficiently. (**#TODO** remove this if we don't achieve to build RISCV correctly!)
