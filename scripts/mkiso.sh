ScriptDirectory="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$ScriptDirectory/mkimg.sh
mkdir -p iso
cp test-build.img iso
xorriso -as mkisofs -R -f -e test-build.img -no-emul-boot -o test-build.iso iso
rm -r iso
