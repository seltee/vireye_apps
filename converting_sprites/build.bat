..\converter.exe ground.png -w10 sprite.png
clang++ --target=thumb -mthumb -mfloat-abi=soft -fdeclspec -c -m32 -Os converting_sprites.cpp pallete.c
..\linker converting_sprites.o pallete.o converting_sprites.vex
copy converting_sprites.vex converting_sprites.bin
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -P converting_sprites.bin 0x08006000
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -Rst
pause