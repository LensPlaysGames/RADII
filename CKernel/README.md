# An Example `C` Kernel
This could be used as a starting point for writing a kernel in the 
`C` programming language based on the RADII bootloader.

### Building
On Windows, enter a linux environment using the Windows Subsystem for Linux (WSL).

The appropriate compiler should have been installed with your distribution, but if not, here's how to install it on debian distributions:
```bash
sudo apt install gcc
```

Ensure that compiler is the correct one by running the following:
```bash
gcc -dumpmachine
```
The command above should produce the following output:
```bash
x86_64-linux-gnu
```

NOTE: If your OS is Windows and you prefer not to use WSL, [Cygwin](https://www.cygwin.com/) is most likely your best bet for this type of compiler.

Once the toolchain is installed, invoke [CMake](https://cmake.org/) to generate a build system:
```bash
cd /Path/to/RADII/CKernel/
cmake -S . -B out
```

When the build system is invoked, it generates the output file from the local source code specified in [CMakeLists.txt](CMakeLists.txt).

The default build system on Linux (and WSL) is GNU Make. Make can then be invoked to build an ELF file, like so:
```bash
cd /Path/to/RADII/CKernel/out/
make
```

If all goes well, a `kernel.elf` file should be generated within the `CKernel` directory. This can be installed into the root file system and the bootloader will then need to find and load it at boot using firmware only.

### Running
To help with the task of actually running and testing this kernel, helper script(s) are included in this directory to build a bootable UEFI disk image using the RADII bootloader. These scripts require that the RADII bootloader binaries are already present (see the build section of the README in the root of the repository).

The scripts included within the `CKernel` directory work exactly like the ones in the `scripts` directory, except for the fact that functionality to support a kernel is added.

The generated images over-ride the ones normally used to test the RADII bootloader by itself, so running them within QEMU or VirtualBox is exactly the same steps as outlined in the README in the root of the repo.
