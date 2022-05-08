# The RADII Bootloader
RADII is an EFI bootloader written in C, and built with CMake.

The vision for RADII involves an easily customizable bootloader,
  including a boot information structure based on a configuration file.
  This would allow for the ability to adapt the layout of the information
  passed to the kernel based on that specific kernel's needs.

The hope of creating this bootloader is to gain understanding
  of the full framework of an OS, from hardware to userland.

By no means does this bootloader aim to cover the UEFI specification in full,
  but it does implement enough for necessary and useful functionality.

---

### Table of Contents
- [Operating Systems that Use RADII](#OSes-using)
- [Building RADII](#build)
  - [Dependencies](#deps)
  - [Bootloader](#bootloader)
- [Booting RADII](#booting)
  - [Boot Media Generation](#boot-media-generation)
  - [Virtual Machines](#vms)
    - [QEMU](#qemu)
    - [VirtualBox](#virtualbox)
    - [VMWare Workstation Player](#vmware)
---

### Operating Systems that Use this Bootloader <a name="OSes-using"></a>
- *COMING SOON* - [LensorOS](https://github.com/LensPlaysGames/LensorOS)

### Building RADII <a name="build"></a>
- [Dependencies](#deps)
- [Bootloader](#bootloader)

NOTE: Every block of shell commands assumes a starting working directory of this repository, RADII.

#### Dependencies <a name="deps"></a>
Tools required for the build:
- CMake
  - Windows: [CMake download page](https://cmake.org/download/)
  - Debian: `sudo apt install cmake`
- a MinGW x86_64 cross-compiler
  - Windows: [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/)
  - Debian: `sudo apt install binutils-mingw-w64-x86-64 gcc-mingw-w64-x86-64`

NOTE: Both CMake and the cross compiler ***must*** be in the `PATH` environment variable.

With the pre-requisite tools installed and added to the `PATH` environment variable, we are ready to begin.

#### Bootloader <a name="bootloader"></a>
CMake is a build-system generator that takes into account the host machine,
  and supports lots of different build systems (`GNU make`, `Ninja`, etc).

In a terminal, enter the following command to generate a `GNU Make`
  build system that uses our cross compiler in the `bld` subdirectory:
```shell
cmake -G "Unix Makefiles" -S . -B bld
```

To invoke the generated build system (and actually build the bootloader), use the following command:
```shell
cmake --build bld
```

At this point, `bin/main.efi` will have been generated.
  This is the executable EFI application that UEFI hardware is able to run.

### Booting RADII <a name="booting"></a>
- [Boot Media Generation](#boot-media-generation)
- [Virtual Machines](#vms)

#### Boot Media Generation <a name="boot-media-generation"></a>
CMake targets that generate boot media are listed below.
  Nested underneath each are it's dependencies, or programs it requires to work.
  If the dependencies are not found on the system at the time of CMake
  configuration, the targets will not be included in the generated build system.

- `image_dir` -- Directory structure of a UEFI-compatible boot image.
- `image_raw` -- FAT32 UEFI-compatible boot image.
  - `dd` -- Native command on Unix
    - On Windows, use one of the following options:
      - [Cygwin](https://www.cygwin.com/)
      - [MinGW installer to get MSYS coreutils ext package](https://osdn.net/projects/mingw/)
      - [Windows Subsystem for Linux (recommended)](https://docs.microsoft.com/en-us/windows/wsl/about)
  - GNU mtools -- MS/DOS filesystem manipulation
    - [Home Page](https://www.gnu.org/software/mtools/)
    - Debian distros: `sudo apt install mtools`
    - [Pre-built binaries for Windows](https://github.com/LensPlaysGames/mtools/releases)

---

#### Virtual Machines <a name="vms"></a>

- QEMU <a name="qemu"></a>

  [Get QEMU](https://www.qemu.org/download/)

  Invoke a CMake target to run QEMU, launching the RADII bootloader upon startup:
``` shell
cmake --build bld -t run_qemu
```

- Targets and their boot media:

  - `run_qemu`

    Launch QEMU straight from a directory that mimics an EFI System boot partition.

  - `runimg_qemu`

    Create a disk image file containing the boot media, then launch QEMU from that.

NOTE: To disable automatic building of executables and boot media, add `_nodeps` to the end of the target name.
  This launches QEMU from existing boot media, if it exists, allowing boot media to be replaced and tested using these targets.

---

- VirtualBox <a name="virtualbox"></a>

  [Get VirtualBox](https://www.virtualbox.org/wiki/Downloads)

1. Click the `New` button to create a new virtual machine.
2. In the pop-up window, give the new virtual machine a name and a file path you are comfortable with.
3. Select Type of `Other` and Version of `Other/Unknown (64-bit)`.
4. Leave the memory size how it is; 64MB is plenty at this time.
5. Select the `Do not add a virtual hard disk` option.
6. Click the `Create` button to create the new virtual machine.
7. Select the new VM in the list on the left, then click the `Settings` button.
8. Navigate to `System` within the list on the left.
    1. Change Chipset to `ICH9`.
    2. Enable Extended Feature `Enable EFI (special OSes only)`.
9. Navigate to `Storage` within the list on the left.
    1. Right click the default controller (`IDE`), and select `Remove Controller`.
    2. Right click the area labeled `Storage Devices`, and select `AHCI (SATA)`.
    3. Right click the new AHCI storage controller, and select either `Optical Drive` or
       `Hard Disk` depending on whether you'd like to boot from the `.iso` or `.bin`, respectively.
    4. Click `Add` in the new Virtual Media Selector window that pops up.
    5. Browse to the `bin` subdirectory and, depending on whether `Optical Drive`
       or `Hard Disk` was selected, choose either the `.iso` or the `.bin` disk image file.
10. Navigate to `Network` within the list on the left.
    1. Disable all network adapters.

---

- VMWare Workstation Player <a name="vmware"></a>

  [Get VMWare Workstation Player](https://www.vmware.com/products/workstation-player.html)

1. Select `Home` in the list on the left side. Click `Create a New Virtual Machine` on the right.
2. Select the `I will install the operating system later.` option.
3. Select a guest OS of `Other`, and a Version of `Other 64-bit`.
4. Give the virtual machine a name and path you are comfortable with. Keep note of the path.
5. It will ask about a disk, but the disk it's asking about won't be used. Click next.
6. The next screen should be an overview of the virtual machine hardware. Click `Customize Hardware...`.
    1. Select `New CD/DVD` on the left, then click `Advanced...` on the right.
    2. Select `SATA`, then click `OK`.
    3. On the right, select `Use ISO image file`, and then click `Browse...`.
    4. Select the `.iso` image file located in the `bin` subdirectory.
    5. Select the hard drive that we skipped configuring in the list on the left.
    6. Remove the hard drive using the `Remove` button near the bottom center.
    7. Remove any and all network adapters and sound cards in the same manner.
    8. Click `Close` in the bottom right to close the hardware configuration window.
7. Click `Finish`.
8. Navigate to the path specified in step \#5, where the virtual machine is located.
    1. Open the file ending with `.vmx` in a text editor.
    2. Add the following line of text: `firmware="efi"`.
    3. Save the file, then close it.

You will have to select `UEFI Shell` once VMware Workstation
  boots into LensorOS (even if it says something like \"Unsupported\").

---
