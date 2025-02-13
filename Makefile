# Makefile for QEMU and Kate operations

.PHONY: help configure-qemu compile-qemu run-qemu

help:
	@echo "Available targets:"
	@echo "  build-qemu       - Configure and build QEMU."
	@echo "  run-qemu         - Run QEMU."
	@echo "  sync-files       - Sync QEMU modified files"
	@echo "  help             - Show this help message."

build-qemu:
	@echo "Configuring and Building QEMU..."
	/mnt/build-qemu.sh

run-qemu:
	@echo "Running QEMU..."
	/mnt/buildroot/output/images/start-qemu.sh

sync-files:
	@echo "Synching files..."
	/mnt/syncfiles.sh
