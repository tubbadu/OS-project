# QEMU & Buildroot Setup Guide

This guide provides step-by-step instructions to set up QEMU and Buildroot on Ubuntu, modify necessary files, and use the provided Makefile for automation.

## Install Required Packages

To ensure a smooth setup, install the necessary dependencies using the following command:

```
sudo apt update && sudo apt install -y $(cat requirements.txt)  
```

## Makefile Guide for QEMU and Buildroot

This repository provides a Makefile to automate the process of building and running QEMU and Buildroot for ARM (aarch64) architecture.

### Available Makefile Targets

Run `make help` to see available targets:

```text
Available targets:
  env                   - Create .env file.
  clone-repos           - Clone QEMU and Buildroot repositories.
  create-setup          - Create initial setup, cloning both QEMU and Buildroot.
  build-buildroot       - Build Buildroot for.
  build-qemu            - Configure and build QEMU.
  run-qemu              - Run QEMU.
  apply-mods            - Sync modified files.
  build-kernel-module   - Build kernel module.
  build-test            - Build kernel module test.
  copy-test             - Copy test executable to root filesystem.
  help                  - Prints this help message.
```

## Build Instructions:

- Clone this repository:

```
git clone https://github.com/tubbadu/OS-project.git
cd OS-project
```

- Create `.env` file:
   
```
make env
```

- Clone [Buildroot](https://github.com/buildroot/buildroot) and [QEMU](https://github.com/qemu/qemu) repositories (if you already cloned them, you can move them inside `OS-project` and skip this step):
  
```
make clone-repos
```

- Create the setup for the build environment: 
  
```
make create-setup
```

- Apply the modifications to the source files of QEMU and Buildroot:
   
```
make apply-mods
```

- Build Buildroot (it will take a while):
   
```
make build-buildroot
```

- Build QEMU with the FFT_CORE (it will take a while):
  
```
make build-qemu
```
  
- Build the kernel module inside Buildroot:

```
make build-kernel-module
```

- Cross-compile the test:

```
make build-test
```
- Copy the test inside the root filesystem of the buildroot image

```
make copy-test
```
  
- Run the Buildroot image with the modified QEMU:
   
```
make run-qemu
```

  When prompted for a login, type `root` and press Enter. Congrats, you are now inside the VM!
    
  You can install the kernel module with:
  
```
/opt/install-kernel-module.sh
```

To check that everything is workig correctly you can run the test:
```
/opt/fft_test
```

To run the example program:
```
/opt/fft_example
```
  
