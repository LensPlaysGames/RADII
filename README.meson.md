# Meson build system

### Table of Contents

- [Dependencies](#deps)
- [Building](#build)

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

### Generating boot media <a name="boot-media"></a>

Unlike CMake, these targets are generated whether
you have the necessary dependencies or not,
so be sure to have the dependencies :^).

- `image-dir` \-\- Generate a boot directory structure with bootloader installed.
  - `mkdir -p`
    - Native on Unix
    - Windows
      - [Cygwin](https://www.cygwin.com/install.html)
      - [MinGW through TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/)
      - [MSYS2](https://www.msys2.org/)
      - Some shells come with their own version shipped, i.e. `nushell`.

### Running <a name="run"></a>

Like CMake, targets to run QEMU are generated.

[QEMU](https://www.qemu.org/download/)

- `qemu-dir`
  - `image-dir`

For other VMs, follow the same instructions found in [README.md](README.md#vms)
