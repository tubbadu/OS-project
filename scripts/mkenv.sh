

cat <<EOF > .env
BASEDIR="$(pwd)"
QEMU_ARM="\$BASEDIR/qemu/build-arm/aarch64-softmmu/qemu-system-aarch64"
QEMU_RISCV="\$BASEDIR/qemu/build-riscv/riscv64-softmmu/qemu-system-riscv64"
EOF