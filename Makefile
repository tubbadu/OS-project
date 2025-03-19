.PHONY: help build-qemu-riscv run-qemu-riscv apply-mods build-qemu-arm run-qemu-arm build-buildroot-arm build-buildroot-riscv env clone-repos create-setup-arm build-kernel-module-arm build-test copy-test all-riscv all-arm
# TODO also remove useless phony
help:
	@echo "Available targets:"
	@echo "  env                   - Create .env file."
	@echo "  clone-repos           - Clone QEMU and Buildroot repositories."
	@echo "  create-setup          - Create initial setup, cloning both QEMU and Buildroot." # CHECK
	@echo "  build-buildroot       - Build Buildroot for."
	@echo "  build-qemu            - Configure and build QEMU."
	@echo "  run-qemu              - Run QEMU."
	@echo "  apply-mods            - Sync modified files."
	@echo "  build-kernel-module   - Build kernel module."
	@echo "  build-test            - Build kernel module test."
	@echo "  copy-test             - Copy test executable to root filesystem."
	@echo "  help                  - Prints this help message."


apply-mods:
	@echo "Synching files..."
	python3 scripts/apply-mods.py

build-qemu:
	@echo "Configuring and Building QEMU..."
	bash scripts/build-qemu.sh

run-qemu:
	@echo "Running QEMU..."
	bash scripts/start-qemu.sh

build-buildroot:
	@echo "Building Buildroot..."
	make -C buildroot qemu_aarch64_virt_defconfig
	make -C buildroot

env:
	@echo "Creating .env file..."
	bash scripts/mkenv.sh

clone-repos:
	@echo "Cloning QEMU and Buildroot repositories..."
	bash scripts/clone-repos.sh

create-setup:
	@echo "Creating setup..."
	bash scripts/create-setup.sh

build-kernel-module:
	@echo "Building kernel module..."
	make -C buildroot fft_module-dirclean
	make -C buildroot

build-test:
	@echo "Building kernel module test and example programs..."
	bash scripts/build-fft-test.sh fft_test
	bash scripts/build-fft-test.sh fft_example
	

copy-test:
	@echo "Copying test executable to root filesystem..."
	bash scripts/copy-test-in-rootfs.sh


x: apply-mods build-qemu-arm build-kernel-module-arm build-test copy-test run-qemu-arm # TODO remove at the end

xnoqemu: apply-mods build-kernel-module-arm build-test copy-test run-qemu-arm
