#!/bin/bash
ScriptDirectory="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TopLevelDirectory="$ScriptDirectory/.."
BuildDirectory="$TopLevelDirectory/bin"
OVMFDirectory="$TopLevelDirectory/OVMFbin"
if ! [ -f $OVMFDirectory/OVMF_VARS_RADII.fd ] ; then
    cp $OVMFDirectory/OVMF_VARS-pure-efi.fd $OVMFDirectory/OVMF_VARS_RADII.fd
fi
qemu-system-x86_64 \
    -d cpu_reset \
    -machine q35 \
    -cpu qemu64 \
    -m 100M \
    -serial stdio \
    -vga cirrus \
    -soundhw pcspk \
    -rtc base=localtime,clock=host,driftfix=none \
    -drive format=raw,file=$BuildDirectory/test-build.img \
    -drive if=pflash,format=raw,unit=0,file=$OVMFDirectory/OVMF_CODE-pure-efi.fd,readonly=on \
    -drive if=pflash,format=raw,unit=1,file=$OVMFDirectory/OVMF_VARS_RADII.fd -net none
