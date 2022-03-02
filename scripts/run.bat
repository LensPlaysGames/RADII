SET BuildDirectory=%0/../../bin
SET OVMFbin=%0/../../OVMFbin
SET BuildDirectory=%BuildDirectory:"=%
SET OVMFbin=%OVMFbin:"=%
IF NOT EXIST %OVMFbin%/OVMF_VARS_RADII.fd cp %OVMFbin%/OVMF_VARS-pure-efi.fd %OVMFbin%/OVMF_VARS_RADII.fd
qemu-system-x86_64 ^
 -d cpu_reset ^
 -machine q35 ^
 -cpu qemu64 ^
 -m 100M ^
 -serial stdio ^
 -vga cirrus ^
 -soundhw pcspk ^
 -net none ^
 -rtc base=localtime,clock=host,driftfix=none ^
 -drive format=raw,file=%BuildDirectory%/test-build.img ^
 -drive if=pflash,format=raw,unit=0,file=%OVMFbin%/OVMF_CODE-pure-efi.fd,readonly=on ^
 -drive if=pflash,format=raw,unit=1,file=%OVMFbin%/OVMF_VARS_RADII.fd
