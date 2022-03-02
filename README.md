# The RADII Bootloader
This is not only a library of wrappers around UEFI, the leading hardware supported firmware interface on modern desktop computers, but also an ELF loader that will jump to `kernel.elf`'s `_start` function. 

The hope of creating this bootloader is to gain understanding (and control) of the full framework of an OS, from hardware to userland. 

By no means does this cover the UEFI specification in full, but it does implement enough for useful functionality (ie. `printf`, passing memory map + RSDP (XSDT?) to kernel, etc).

---

### Table of Contents
- [UEFI Resources](#Resources)
- [Operating Systems that Use this Bootloader](#OS-using)
- [Building this Bootloader](#build)
  - [Windows](#build-windows)
  - [Linux](#build-debian-linux)
- [Testing](#testing)
  - [QEMU](#test-qemu)
  - [VirtualBox](#test-virtualbox)
---

### UEFI Resources <a name="Resources"></a>
- UEFI Spec. Version 2.9 [pdf](https://uefi.org/sites/default/files/resources/UEFI_Spec_2_9_2021_03_18.pdf)

### Operating Systems that Use this Bootloader <a name="OS-using"></a>
- #### *COMING SOON* - [LensorOS](https://github.com/LensPlaysGames/LensorOS)

### Building this Bootloader <a name="build"></a>
#### Table of Contents
- [Windows](#build-windows)
- [Linux](#build-debian-linux)

#### Windows <a name="build-windows"></a>
Tools required for the build:
- [CMake](https://cmake.org/download/)
- [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/)

NOTE: You may use any 64-bit MinGW version of GCC that targets x86_64 and generates `PE32+` executables, but TDM-GCC has been the easiest to use and most reliable for me so far.

On Windows, the installers are not allowed to modify the system environment variables, including `$PATH`. \
`$PATH` is where terminals look for executables to execute that match the name of the command you ran, among other things. \
This variable must be edited to include the `bin` directories of both CMake and TDM-GCC, otherwise the build system will not know how to invoke the tools. \
To edit `$PATH`, do the following (tested on Windows 10):
1. Open the Windows Start Menu, and (without clicking anything) type `environment variables` and select the `Edit the system environment variables` option.
2. Click the small button in the bottom-right: `Environment Variables...`
3. In the new window that pops up, there are two tables; select the `Path` variable within the lower table titled `System variables`.
4. In the bottom right of the `System variables` section, click the `Edit...` button.
5. A new window should appear. Within it is a list of all the different directories that `$PATH` points to. Add a new entry to this list by clicking the `New` button in the top right. Then, just type the absolute path to the `bin` directories of the tools installed (ie. `C:\TDM-GCC-64\bin`, in a new entry: `C:\CMake\bin`. Replace the path with wherever you installed the tools to).

With the pre-requisite tools installed and added to your system's `$PATH`, we are ready to begin. \
First, we must generate a build system that uses our cross compiler. \
Luckily, CMake supports a build system titled `MinGW Makefiles`, and that is exactly what we need.
In a terminal, enter the following commands:
```bash
cd Path\To\RADII\
cmake -G "MinGW Makefiles" -S . -B bld/
```

This will generate the necessary build system that will build the bootloader from source within the `bld` directory. \
In order to build the bootloader using the build system, invoke `mingw32-make` within the build directory:
```bash
cd Path\To\RADII\bld\
mingw32-make.exe
```

At this point, `main.efi.exe` will have been created within the newly created `bin` directory of the repository. As you can tell, there is a slight error in how GCC handles compilation output extension based on host, so you do need to remove the `.exe` from the file name before using it.

#### Linux <a name="build-debian-linux"></a>
Before we begin, we will need a MinGW x86_64 cross-compiler, as well as CMake. \
On debian distributions, the cross-compiler and CMake may be obtained all at once with the following command:
```bash
sudo apt install cmake binutils-mingw-w64-x86-64 gcc-mingw-w64-x86-64
```

To test that it is installed, simply run `x86_64-w64-mingw32-gcc --version` \
If all went well, you should see the version of GCC that the cross compiler is based on, as well as a copyright and license notice.

Next, we will need to generate a build system that we can then invoke to build the source code from our machine. \
To do this, the CMake tool is utilized, a build-system generator that takes into account the host machine, and supports lots of different build systems (`GNU make`, `Ninja`, etc).

Open a terminal, and enter the following commands:
```bash
cd Path/To/RADII/
cmake -S . -B bld/
```

This will generate the default build system for your machine within the `bld` subdirectory. \
On Linux, the default build system is pretty universally `GNU make`, so to build the bootloader, simply run the following:
```bash
cd Path/To/RADII/bld/
make
```

After that, you should see a `bin` directory has been created within the root of the repository. Within that directory `main.efi` can be found, which is ready to be loaded into a FAT32 filesystem at `/EFI/BOOT/bootx64.efi` as per the UEFI spec. V2.9, Table 3-2 After this, you are able to boot from that filesystem and this is the bootloader that will be run, given a valid `.nsh` startup script.

### Testing <a name="testing"></a>
On Windows, I recommend the Windows Subsystem for Linux (WSL2) to be able to run the following exactly as is, but I'm sure an equivalent *could* be done natively.

To build an image that actually uses the bootloader and can be booted from a UEFI environment, I have included some scripts that may be able to help out (or at least point you in the right direction). \
To create and format FAT32 partitions as files (disk images), the following tools are necessary:
```bash
sudo apt install mtools
```

Next, run the included bash script named `mkimg.sh`
```bash
bash Path/To/RADII/mkimg.sh
```

This will generate a 48MB FAT32-formatted image that may be cloned onto an actual boot device (USB, for example), used as a floppy within QEMU flashed with OVMF, or, with a little further processing, as a boot CD-ROM for VirtualBox.

- [QEMU](#test-qemu)
- [VirtualBox](#test-virtualbox)

### QEMU <a name="test-qemu"></a>
Ensure QEMU for x86_64 is installed on your host system:
- [Windows](https://www.qemu.org/download/)
- Linux: `sudo apt install qemu-system-x86`

For ease of use, I've included pre-built OVMF binaries in the `OVMFbin` folder, as well as instructions on how to generate them yourself if you are so inclined, or if the ones in this repository are out of date. These will be used automatically when you run the `run` bash/batch script from the `scripts` directory.

Finally, to run QEMU and boot into the bootloader, use the following commands:
- Windows (remember to run from Windows command line, not WSL)
```bash
Path\To\RADII\scripts\run.bat
```
- Linux
```bash
bash /Path/To/RADII/scripts/run.sh
```

### VirtualBox <a name="test-virtualbox"></a>
Ensure VirtualBox is downloaded on your host system. [link](https://www.virtualbox.org/wiki/Downloads)

Within a linux terminal, download the `xorriso` tool:
```bash
sudo apt install xorriso
```

This tool creates `.iso` files formatted in the ISO-9660 filesystem. This is useful as CD-ROM drives use this file system.

To create a bootable CD-ROM drive from the bootable floppy image, run the following helper script:
```bash
bash /Path/To/RADII/scripts/mkiso.sh
```

Once complete, we will have a file named `test-build.iso` in the `bin/` directory of the repository. To use this as a boot CD-ROM in VirtualBox, complete the following steps:
1. Open VirtualBox.
2. Click the `New` button.
3. Give the VM a name and a file path you are comfortable with.
4. Select Type of `Other` and Version of `Other/Unknown (64-bit)`.
5. Leave the memory size how it is; 64MB is plenty for this bootloader.
6. Select `Do not add a virtual hard disk` option.
7. Click the `Create` button.
8. With the new VM selected within the list on the left, click the `Settings` button.
9. Navigate to `System` within the list on the left.
    1. Change Chipset to `ICH9`.
    2. Enable Extended Feature `Enable EFI (special OSes only)`.
    3. Navigate to the `Processor` tab, and check the `Enable Nested VT-x/AMD-V` checkbox.
10. Navigate to `Storage` within the list on the left.
    1. Click the little blue circle with a plus icon next to `Controller: IDE`
    2. Click the `Add` button in the new window that pops up.
    3. Browse to `Path/To/RADII/bin/test-build.iso` and select it.
    4. Ensure `test-build.iso` is selected within the list, and click the `Choose` button.
11. Navigate to `Network` within the list on the left.
    1. Disable all network adapters.

After all of this has been done, you are ready to click `Start` on the VirtualBox VM; the bootloader should run automatically.
