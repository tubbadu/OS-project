# FFT_CORE

## build instructions:

1. clone this repository: 

	```shell
	git clone https://github.com/tubbadu/OS-project/tree/main
	cd OS-project
	```

2. create `.env` file:

	```shell
	make env
	```

3. clone [Buildroot](https://github.com/buildroot/buildroot) and [QEMU](https://github.com/qemu/qemu) repositories (if you already cloned them, you can move them inside `OS-project` and skip this step):

	```shell
	make clone-repos
	```

4. create the setup for the build environment:

	```shell
	make create-setup-arm
	```

5. build buildroot (it will take a while):

	```shell
	make buildroot-build-arm
	```

6. Insert the FFT_CORE source files in QEMU:

	```shell
	make sync-files
	```

7. build QEMU with the FFT_CORE (it will take a while):
   
	```shell
	make build-qemu-arm
	```

8. run the modified QEMU:

	```shell
	make run-qemu-arm
	```
