# The RADII Bootloader
This is not only a library of wrappers around UEFI, the leading hardware supported firmware interface on modern desktop computers, but also an ELF loader that will jump to `kernel.elf`'s `main` function. 

The hope of creating this bootloader is to gain understanding (and control) of the full framework of an OS, from hardware to userland. 

By no means does this cover the UEFI specification in full, but it does implement enough for useful functionality (ie. `printf`, passing memory map + RSDP (XSDT?) to kernel, etc).

### UEFI Resources
- UEFI Spec. Version 2.9 [pdf](https://uefi.org/sites/default/files/resources/UEFI_Spec_2_9_2021_03_18.pdf)

### Operating Systems that Use this Bootloader
- #### *COMING SOON* - [LensorOS](https://github.com/LensPlaysGames/LensorOS)