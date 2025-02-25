.PHONY: help build-qemu-riscv run-qemu-riscv sync-files build-qemu-arm run-qemu-arm build-buildroot-arm build-buildroot-riscv env clone-repos create-setup-arm all-riscv all-arm

help:
	@echo "Available targets:"
	@echo "  env                    - Create .env file."
	@echo "  clone-repos            - Clone QEMU and Buildroot repositories."
	@echo "  create-setup-arm       - Create initial setup for ARM, cloning both QEMU and Buildroot."
	@echo "  build-buildroot-arm    - Build Buildroot for ARM."
	@echo "  build-buildroot-riscv  - Build Buildroot for RISCV."
	@echo "  build-qemu-arm         - Configure and build QEMU for ARM."
	@echo "  build-qemu-riscv       - Configure and build QEMU for RISCV."
	@echo "  run-qemu-arm           - Run QEMU for ARM."
	@echo "  run-qemu-riscv         - Run QEMU for RISCV."
	@echo "  sync-files             - Sync modified files."
	@echo "  all-riscv              - Sync, build, and run QEMU for RISCV."
	@echo "  all-arm                - Sync, build, and run QEMU for ARM."


build-qemu-riscv:
	@echo "Configuring and Building QEMU RISCV..."
	bash scripts/build-qemu-riscv.sh

run-qemu-riscv:
	@echo "Running QEMU RISCV..."
	bash scripts/start-qemu-riscv.sh

sync-files:
	@echo "Synching files..."
	python3 scripts/sync-files.py

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

build-buildroot-riscv:
	@echo "Building Buildroot for RISCV..."
	make -C buildroot-riscv qemu_riscv64_virt_defconfig
	make -C buildroot-riscv

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
	bash scripts/build-fft-test.sh

all-riscv: sync-files build-qemu-riscv  run-qemu-riscv

all-arm: sync-files build-qemu-arm build-kernel-module-arm run-qemu-arm