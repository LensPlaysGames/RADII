ScriptDirectory="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TopLevelDirectory=$ScriptDirectory/..
$ScriptDirectory/mkimg.sh
mkgpt -o $TopLevelDirectory/bin/test-build.bin --part $TopLevelDirectory/bin/test-build.img --type system
