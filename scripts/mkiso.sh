ScriptDirectory="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$ScriptDirectory/mkimg.sh
mkdir -p $ScriptDirectory/iso
cp $ScriptDirectory/../bin/test-build.img $ScriptDirectory/iso
xorriso -as mkisofs -R -f -e test-build.img -no-emul-boot -o $ScriptDirectory/../bin/test-build.iso $ScriptDirectory/iso
rm -r $ScriptDirectory/iso
