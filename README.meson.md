# Meson build system

### Table of Contents

- [Dependencies](#deps)
- [Building](#build)
  - [Generating Boot Media](#boot-media)
- [Running](#run)
- [Common Errors](#err)

### Dependencies <a name="deps"></a>

- a MinGW x86_64 cross-compiler
  - Debian: `sudo apt install binutils-mingw-w64-x86-64 gcc-mingw-w64-x86-64`
  - Windows (only one of the following):
    - [Cygwin](https://www.cygwin.com/install.html)
    - [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/)
    - [MSYS2](https://www.msys2.org/)

- [Python](https://www.python.org/downloads/)

- [Ninja](https://ninja-build.org/)

- Meson:
  - [Website](https://mesonbuild.com/)
  - [GitHub](https://github.com/mesonbuild/meson)

  At the time of writing (`Meson 0.62.1`), it is necessary to use the latest
  off of source control. The distributed binaries do not have a necessary
  patch included, but my PR has been merged upstream.

---

### Building <a name="build"></a>

NOTE: Shell commands expect the starting working
directory in the directory this file is in.

#### Generate a build tree

``` shell
python path/to/meson.py . builddir --cross-file x86_64-pe.txt
```

#### Invoke the build

``` shell
python path/to/meson.py compile -C builddir [target]
```

`target` specifies a Ninja target to run.
To see all available targets, use the following command:

``` shell
ninja -C builddir -t targets
```

#### Generating boot media <a name="boot-media"></a>

Unlike CMake, these targets are generated whether
you have the necessary dependencies or not,
so be sure to have the dependencies :^).

- `image-dir` \-\- Generate a boot directory structure with bootloader installed.
  - Python

---

### Running <a name="run"></a>

Like CMake, targets to run QEMU are generated.

[QEMU](https://www.qemu.org/download/)

- `qemu-dir`
  - `image-dir`

For other VMs, follow the same instructions found in [README.md](README.md#vms)

---

### Common errors <a name="err"></a>

#### fatal error: resource_table.h: No such file or directory

This is either due to `resource_table.c` or `resource_table.h` being missing.

This is common because Meson does not ensure existence
of these files at build-time invocation, but instead generates
them once when the build tree is generated. This means if they
are removed in any way, they will not be rebuilt automatically,
unlike the CMake build system which is smarter about dependencies.

- **Solutions:**

  1. Reconfigure Meson build tree

``` shell
cd builddir
python path/to/meson --reconfigure
```

  2. Wipe and rebuild build tree

``` shell
python path/to/meson.py . builddir --cross-file x86_64-pe.ini --wipe
```
