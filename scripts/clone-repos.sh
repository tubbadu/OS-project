FOLDERQEMU="qemu"
FOLDERBUILDROOT="buildroot"

if [ ! -d "$FOLDERQEMU" ]; then
    git clone https://github.com/qemu/qemu.git
fi

if [ ! -d "$FOLDERBUILDROOT" ]; then
	git clone https://github.com/buildroot/buildroot.git
fi