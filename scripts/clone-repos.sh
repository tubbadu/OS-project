FOLDERQEMU="qemu"
FOLDERBUILDROOT="buildroot"
FOLDERBUILDROOTARM="buildroot-arm"

if [ ! -d "$FOLDERQEMU" ]; then
    git clone https://github.com/qemu/qemu.git
# 	cd qemu
# 	git checkout v6.0.0
# 	mkdir build-arm build-riscv
# 	cd ..
fi

if [ ! -d "$FOLDERBUILDROOT" ] && [ ! -d "$FOLDERBUILDROOTARM" ]; then
	git clone https://github.com/buildroot/buildroot.git
# 	mv buildroot buildroot-arm
fi