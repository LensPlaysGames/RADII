@echo -off
mode 80 25

cls
if exist .\EFI\BOOT\main.efi then
 .\EFI\BOOT\main.efi
 goto END
endif

if exist fs0:\EFI\BOOT\main.efi then
 fs0:
 echo Found BOOTloader on fs0:
 EFI\BOOT\main.efi
 goto END
endif

if exist fs1:\EFI\BOOT\main.efi then
 fs1:
 echo Found BOOTloader on fs1:
 EFI\BOOT\main.efi
 goto END
endif

if exist fs2:\EFI\BOOT\main.efi then
 fs2:
 echo Found BOOTloader on fs2:
 EFI\BOOT\main.efi
 goto END
endif

if exist fs3:\EFI\BOOT\main.efi then
 fs3:
 echo Found BOOTloader on fs3:
 EFI\BOOT\main.efi
 goto END
endif

if exist fs4:\EFI\BOOT\main.efi then
 fs4:
 echo Found BOOTloader on fs4:
 EFI\BOOT\main.efi
 goto END
endif

if exist fs5:\EFI\BOOT\main.efi then
 fs5:
 echo Found BOOTloader on fs5:
 EFI\BOOT\main.efi
 goto END
endif

if exist fs6:\EFI\BOOT\main.efi then
 fs6:
 echo Found BOOTloader on fs6:
 EFI\BOOT\main.efi
 goto END
endif

if exist fs7:\EFI\BOOT\main.efi then
 fs7:
 echo Found BOOTloader on fs7:
 EFI\BOOT\main.efi
 goto END
endif

 echo "Unable to find BOOTloader".
 
:END
