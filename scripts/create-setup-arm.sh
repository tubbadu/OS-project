FOLDERQEMU="qemu"
FOLDERBUILDROOT="buildroot"
FOLDERBUILDROOTARM="buildroot-arm"
ERRORMSG="Run 'make clone-repos' to clone the qemu and buildroot repositories in the current folder. If you already cloned them, move them in the current directory and run this command again."

if [ ! -d "$FOLDERQEMU" ]; then
    echo "Error: $FOLDERQEMU does not exist."
    echo "$ERRORMSG"
    exit 1
else
	cd qemu
	git checkout v6.0.0
	mkdir -p build-arm build-riscv
	cd ..
fi

if [ ! -d "$FOLDERBUILDROOTARM" ]; then
    if [ ! -d "$FOLDERBUILDROOT" ]; then
		echo "Error: $FOLDERBUILDROOT or $FOLDERBUILDROOTARM do not exist."
		echo "$ERRORMSG"
		exit 1
	else
		mv "$FOLDERBUILDROOT" "$FOLDERBUILDROOTARM"
	fi
fi
