
#CC = $(CROSS_COMPILE)gcc
CC = "/mnt/buildroot-arm/output/host/bin/aarch64-linux-gcc"
ARCH = arm

obj-m := hello.o
KDIR := /mnt/buildroot-arm/output/build/linux-6.12.9

all:
	$(MAKE) -C $(KDIR) M=$(PWD)
