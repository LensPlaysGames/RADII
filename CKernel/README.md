# An Example `C` Kernel
This could be used as a starting point for writing a kernel in the 
`C` programming language based on the RADII bootloader.

---

### Building <a name="building"></a>
A C compiler targeting `x86_64-linux-gnu` is necessary.
- On Debian: `sudo apt install build-essential`
- For Windows users, it is easiest to just use the
  [Windows Subsystem for Linux (WSL)](https://docs.microsoft.com/en-us/windows/wsl/about),
  rather than trying to build a native, cross-compiling toolchain using something like
  [Cygwin](https://www.cygwin.com/).

NOTE: Given shell commands are expected to have this directory, `CKernel`, as the working directory.

Once the toolchain is installed, invoke [CMake](https://cmake.org/) to generate a build system:
```shell
cmake -S . -B out
```

When the build system is invoked, it generates the
  output executable file from the local source code.

To invoke the generated build system, and create the kernel executable, use the following command:
```shell
cmake --build out
```

If all goes well, `kernel.elf` should be generated within the `CKernel` directory.
  This can be installed into the root file system and the bootloader will
  then need to find and load it at boot using firmware only (see [Running](#running)).

---

### Running <a name="running"></a>
Following is a list of CMake targets and the dependencies required for them to work.

- `image_raw` \-\- Generate `test-build.img` with `kernel.elf` installed in root.
  - `dd` \-\- Native command on Unix
  - GNU mtools \-\- MS/DOS filesystem manipulation
    - [Home Page](https://www.gnu.org/software/mtools/)
    - Debian distros: `sudo apt install mtools`

To help with the task of actually running and testing this kernel,
  helper script(s) are included in this directory to build a bootable
  UEFI disk image using the RADII bootloader.
  These scripts require that the RADII bootloader binaries are already
  present (see the build section of the README in the root of the repository).

The scripts included within the `CKernel` directory work exactly like the ones in the `scripts` directory, except for the fact that functionality to support a kernel is added.

The generated images over-ride the ones normally used to test the RADII bootloader by itself, so running them within QEMU or VirtualBox is exactly the same steps as outlined in the README in the root of the repo.

---
