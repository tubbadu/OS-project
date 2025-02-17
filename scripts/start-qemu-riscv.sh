#!/bin/sh
source .env
cd buildroot-riscv/output/images/


BINARIES_DIR="${0%/*}/"
# shellcheck disable=SC2164
cd "${BINARIES_DIR}"

mode_serial=false
mode_sys_qemu=false
while [ "$1" ]; do
    case "$1" in
    --serial-only|serial-only) mode_serial=true; shift;;
    --use-system-qemu) mode_sys_qemu=true; shift;;
    --) shift; break;;
    *) echo "unknown option: $1" >&2; exit 1;;
    esac
done

if ${mode_serial}; then
    EXTRA_ARGS='-nographic'
else
    EXTRA_ARGS=''
fi

if ! ${mode_sys_qemu}; then
    export PATH="/mnt/buildroot/output/host/bin:${PATH}"
fi

# exec qemu-system-riscv64 -M virt -bios fw_jump.bin -kernel Image -append "rootwait root=/dev/vda ro" -drive file=rootfs.ext2,format=raw -netdev user,id=net0 -device virtio-net-device,netdev=net0 -nographic  ${EXTRA_ARGS} "$@"
  exec $QEMU_RISCV -M virt -bios fw_jump.bin -kernel Image -append "rootwait root=/dev/vda ro" -drive file=rootfs.ext2,format=raw,id=hd0 -device virtio-blk-device,drive=hd0 -netdev user,id=net0 -device virtio-net-device,netdev=net0 -nographic
