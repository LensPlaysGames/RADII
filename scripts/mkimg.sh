#!/bin/bash
ImageName="test-build"
ScriptDirectory="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TopLevelDirectory="$ScriptDirectory/.."
BuildDirectory="$TopLevelDirectory/bin"
BootloaderEFI="$BuildDirectory/main.efi"

run(){
    set -x
    "$@"
    { set +x; } 2>/dev/null
}

run mkdir -p $BuildDirectory
run dd if=/dev/zero of=$BuildDirectory/$ImageName.img count=93750
run mformat -i $BuildDirectory/$ImageName.img -F -v "bootdisk" ::
echo "    Created FAT32 bootable disk image."
run mmd -i $BuildDirectory/$ImageName.img ::/EFI
run mmd -i $BuildDirectory/$ImageName.img ::/EFI/BOOT
echo "    Directories initialized."
run mcopy -i $BuildDirectory/$ImageName.img $BootloaderEFI ::/EFI/BOOT
run mcopy -i $BuildDirectory/$ImageName.img $ScriptDirectory/startup.nsh ::
echo "    Resources copied."
