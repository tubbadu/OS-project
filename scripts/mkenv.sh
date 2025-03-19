

cat <<EOF > .env
export PATH=\$(echo "\$PATH" | tr -d ' \t\n') # needed for WSL 
BASEDIR="$(pwd)"
QEMU_ARM="\$BASEDIR/qemu/build/aarch64-softmmu/qemu-system-aarch64"
EOF