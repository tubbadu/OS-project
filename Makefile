# Makefile for QEMU and Buildroot with both ARM and RISCV architectures

.PHONY: help build-qemu-riscv run-qemu-riscv sync-files build-qemu-arm run-qemu-arm all-riscv all-arm all create-setup-arm buildroot-build-arm

help:
	@echo "Available targets:"
	@echo "  build-qemu-riscv      - Configure and build QEMU for RISCV."
	@echo "  run-qemu-riscv        - Run QEMU for RISCV."
	@echo "  build-qemu-arm        - Configure and build QEMU for ARM."
	@echo "  run-qemu-arm          - Run QEMU for ARM."
	@echo "  sync-files            - Sync QEMU modified files."
	@echo "  all-riscv             - Sync, build, and run QEMU for RISCV."
	@echo "  all-arm               - Sync, build, and run QEMU for ARM."
	@echo "  all                   - Sync, build, and run QEMU for ARM."
	@echo "  create-setup-arm      - Create initial setup for ARM, cloning both Qemu and Buildroot."
	@echo "  buildroot-build-arm   - Build Buildroot for ARM."

build-qemu-riscv:
	@echo "Configuring and Building QEMU RISCV..."
	bash build-qemu-riscv.sh

run-qemu-riscv:
	@echo "Running QEMU RISCV..."
	bash start-qemu-riscv.sh

sync-files:
	@echo "Synching files..."
	python3 sync-files.py

build-qemu-arm:
	@echo "Configuring and Building QEMU ARM..."
	bash build-qemu-arm.sh

run-qemu-arm:
	@echo "Running QEMU ARM..."
	bash start-qemu-arm.sh

all-riscv: sync-files build-qemu-riscv run-qemu-riscv

all-arm: sync-files build-qemu-arm run-qemu-arm

all: all-arm # debug only, TODO remove


create-setup-arm:
	@echo "Creating setup for ARM..."
	bash create-setup-arm.sh

buildroot-build-arm:
	@echo "Building buildroot for ARM..."
	cd buildroot-arm
	make qemu_aarch64_virt_defconfig
	make