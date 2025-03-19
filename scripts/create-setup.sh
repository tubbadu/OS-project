FOLDERQEMU="qemu"
ERRORMSG="Run 'make clone-repos' to clone the qemu and buildroot repositories in the current folder. If you already cloned them, move them in the current directory and run this command again."

if [ ! -d "$FOLDERQEMU" ]; then
    echo "Error: $FOLDERQEMU does not exist."
    echo "$ERRORMSG"
    exit 1
else
	cd qemu
	git checkout v6.0.0
	mkdir -p build
	cd ..
fi
