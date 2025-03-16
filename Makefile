.PHONY: help build-qemu-riscv run-qemu-riscv apply-mods build-qemu-arm run-qemu-arm build-buildroot-arm build-buildroot-riscv env clone-repos create-setup-arm build-kernel-module-arm build-test copy-test all-riscv all-arm

help:
	@echo "Available targets:"
	@echo "  env                       - Create .env file."
	@echo "  clone-repos               - Clone QEMU and Buildroot repositories."
	@echo "  create-setup-arm          - Create initial setup for ARM, cloning both QEMU and Buildroot."
	@echo "  build-buildroot-arm       - Build Buildroot for ARM."
# 	@echo "  build-buildroot-riscv     - Build Buildroot for RISCV."
	@echo "  build-qemu-arm            - Configure and build QEMU for ARM."
# 	@echo "  build-qemu-riscv          - Configure and build QEMU for RISCV."
	@echo "  run-qemu-arm              - Run QEMU for ARM."
# 	@echo "  run-qemu-riscv            - Run QEMU for RISCV."
	@echo "  apply-mods                - Sync modified files."
	@echo "  build-kernel-module-arm   - Build kernel module for ARM."
	@echo "  build-test                - Build kernel module test."
	@echo "  copy-test                 - Copy test executable to root filesystem."
# 	@echo "  all-riscv                 - Sync, build, and run QEMU for RISCV."
# 	@echo "  all-arm                   - Sync, build, and run QEMU for ARM."


# build-qemu-riscv:
# 	@echo "Configuring and Building QEMU RISCV..."
# 	bash scripts/build-qemu-riscv.sh
# 
# run-qemu-riscv:
# 	@echo "Running QEMU RISCV..."
#	bash scripts/start-qemu-riscv.sh

apply-mods:
	@echo "Synching files..."
	python3 scripts/apply-mods.py

build-qemu-arm:
	@echo "Configuring and Building QEMU ARM..."
	bash scripts/build-qemu-arm.sh

run-qemu-arm:
	@echo "Running QEMU ARM..."
	bash scripts/start-qemu-arm.sh

build-buildroot-arm:
	@echo "Building Buildroot for ARM..."
	make -C buildroot-arm qemu_aarch64_virt_defconfig
	make -C buildroot-arm
# 
# build-buildroot-riscv:
# 	@echo "Building Buildroot for RISCV..."
# 	make -C buildroot-riscv qemu_riscv64_virt_defconfig
# 	make -C buildroot-riscv

env:
	@echo "Creating .env file..."
	bash scripts/mkenv.sh

clone-repos:
	@echo "Cloning QEMU and Buildroot repositories..."
	bash scripts/clone-repos.sh

create-setup-arm:
	@echo "Creating setup for ARM..."
	bash scripts/create-setup-arm.sh

# build-kernel-module:
# 	@echo "Building FFT module..."
# 	bash scripts/build-fft-module.sh

# withkernel_start-qemu:
# 	@echo "Running QEMU with kernel module..."
# 	bash scripts/withkernel_start-qemu.sh
	
build-kernel-module-arm:
	@echo "Building kernel module..."
	make -C buildroot-arm fft_module-dirclean
	make -C buildroot-arm

build-test:
	@echo "Building kernel module test..."
	bash scripts/build-fft-test.sh fft_test
	bash scripts/build-fft-test.sh fft_example
	

copy-test:
	@echo "Copying test executable to root filesystem..."
	bash scripts/copy-test-in-rootfs.sh

# all-riscv: apply-mods build-qemu-riscv  run-qemu-riscv

# all-arm: apply-mods build-qemu-arm build-kernel-module-arm run-qemu-arm

x: apply-mods build-qemu-arm build-kernel-module-arm build-test copy-test run-qemu-arm

xnoqemu: apply-mods build-kernel-module-arm build-test copy-test run-qemu-arm
