source .env

mkdir -p "$BASEDIR/rootfs"
sudo mount "$BASEDIR/buildroot-arm/output/images/rootfs.ext4" "$BASEDIR/rootfs"
sudo cp "$BASEDIR/test/fft_test" "$BASEDIR/rootfs/opt"
sudo umount "$BASEDIR/rootfs"
