source .env

mkdir -p "$BASEDIR/rootfs"
sudo mount "$BASEDIR/buildroot/output/images/rootfs.ext4" "$BASEDIR/rootfs"
sudo cp -r \
	"$BASEDIR/test/fft_test" \
	"$BASEDIR/test/fft_example" \
	"$BASEDIR/test/data" \
	"$BASEDIR/test/install-kernel-module.sh" \
	"$BASEDIR/rootfs/opt"
sudo umount "$BASEDIR/rootfs"
