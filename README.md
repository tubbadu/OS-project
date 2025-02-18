# FFT_CORE

## build instructions:

1. install the needed packages:
   
   ```shell
   sudo apt install build-essential bzip2 ninja-build git  python3-setuptools
   ```

2. clone this repository: 
   
   ```shell
   git clone https://github.com/tubbadu/OS-project.git
   cd OS-project
   ```

3. create `.env` file:
   
   ```shell
   make env
   ```

4. clone [Buildroot](https://github.com/buildroot/buildroot) and [QEMU](https://github.com/qemu/qemu) repositories (if you already cloned them, you can move them inside `OS-project` and skip this step):
   
   ```shell
   make clone-repos
   ```

5. create the setup for the build environment:
   
   ```shell
   make create-setup-arm
   ```

6. build buildroot (it will take a while):
   
   ```shell
   make buildroot-build-arm
   ```

7. Insert the FFT_CORE source files in QEMU:
   
   ```shell
   make sync-files
   ```

8. build QEMU with the FFT_CORE (it will take a while):
   
   ```shell
   make build-qemu-arm
   ```

9. run the modified QEMU:
   
   ```shell
   make run-qemu-arm
   ```
   
   When prompted for a login, type `root` and press Enter. Congrats, you are now inside the VM!
