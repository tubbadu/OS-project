# Makefile for QEMU and Buildroot with both ARM and RISCV architectures

.PHONY: help build-qemu-riscv run-qemu-riscv sync-files build-qemu-arm run-qemu-arm all-riscv all-arm all create-setup-arm buildroot-build-arm buildroot-build-riscv env clone-repos

help:
	@echo "Available targets:"
	@echo "  env                    - Create .env file."
	@echo "  clone-repos            - Clone QEMU and Buildroot repositories."
	@echo "  create-setup-arm       - Create initial setup for ARM, cloning both QEMU and Buildroot."
	@echo "  buildroot-build-arm    - Build Buildroot for ARM."
	@echo "  buildroot-build-riscv  - Build Buildroot for RISCV."
	@echo "  build-qemu-arm         - Configure and build QEMU for ARM."
	@echo "  build-qemu-riscv       - Configure and build QEMU for RISCV."
	@echo "  run-qemu-arm           - Run QEMU for ARM."
	@echo "  run-qemu-riscv         - Run QEMU for RISCV."
	@echo "  sync-files             - Sync QEMU modified files." # TODO find a better name for this target
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

all-riscv: sync-files build-qemu-riscv run-qemu-riscv

all-arm: sync-files build-qemu-arm run-qemu-arm

buildroot-build-arm:
	@echo "Building Buildroot for ARM..."
	cd buildroot-arm && make qemu_aarch64_virt_defconfig && make

buildroot-build-riscv:
	@echo "Building Buildroot for RISCV..."
	cd buildroot-arm
	make qemu_riscv64_virt_defconfig
	make

env:
	@echo "Creating .env file..."
	bash scripts/mkenv.sh

clone-repos:
	@echo "Cloning QEMU and Buildroot repositories..."
	bash scripts/clone-repos.sh

create-setup-arm:
	@echo "Creating setup for ARM..."
	bash scripts/create-setup-arm.sh

build-fft-module:
	@echo "Building FFT module..."
	bash scripts/build-fft-module.sh

withkernel_start-qemu:
	@echo "Running QEMU with kernel module..."
	bash scripts/withkernel_start-qemu.sh
