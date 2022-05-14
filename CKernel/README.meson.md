# Example `C` Kernel using Meson

### Table of Contents

- [Building](#building)

### Building <a name="building"></a>

- [Dependencies](#deps)
- [Build System](#build-system)
- [Boot Media Generation](#boot-media)

#### Dependencies <a name="deps"></a>

---

### Building <a name="build"></a>

#### Generating boot media <a name="boot-media"></a>

Targets to generate boot media with the
kernel installed are automatically generated.

- `image-dir` \-\- Generate a boot directory structure with bootloader installed.
  - `mkdir -p`
    - Native on Unix
    - Windows
      - [Cygwin](https://www.cygwin.com/install.html)
      - [MinGW through TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/)
      - [MSYS2](https://www.msys2.org/)
      - Some shells come with their own version shipped, i.e. `nushell`.

---

### Running <a name="run"></a>

[QEMU](https://www.qemu.org/download/)

- `qemu-dir`
  - `image-dir`

For other VMs, follow the same instructions found in [README.md](README.md#vms)
