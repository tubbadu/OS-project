# Makefile for QEMU and Buildroot with both ARM and RISCV architectures

.PHONY: help build-qemu-riscv run-qemu-riscv sync-files build-qemu-arm run-qemu-arm all-riscv all-arm all

help:
	@echo "Available targets:"
	@echo "  build-qemu-riscv - Configure and build QEMU for RISCV."
	@echo "  run-qemu-riscv   - Run QEMU for RISCV."
	@echo "  build-qemu-arm   - Configure and build QEMU for ARM."
	@echo "  run-qemu-arm     - Run QEMU for ARM."
	@echo "  sync-files       - Sync QEMU modified files."
	@echo "  all-riscv        - Sync, build, and run QEMU for RISCV."
	@echo "  all-arm          - Sync, build, and run QEMU for ARM."
	@echo "  all              - Sync, build, and run QEMU for ARM."

build-qemu-riscv:
	@echo "Configuring and Building QEMU RISCV..."
	/mnt/build-qemu-riscv.sh

run-qemu-riscv:
	@echo "Running QEMU RISCV..."
	/mnt/buildroot-riscv/output/images/start-qemu.sh

sync-files:
	@echo "Synching files..."
	python3 /mnt/sync-files.py

build-qemu-arm:
	@echo "Configuring and Building QEMU ARM..."
	/mnt/build-qemu-arm.sh

run-qemu-arm:
	@echo "Running QEMU ARM..."
	/mnt/buildroot-arm/output/images/start-qemu.sh

all-riscv: sync-files build-qemu-riscv run-qemu-riscv

all-arm: sync-files build-qemu-arm run-qemu-arm

all: all-arm
