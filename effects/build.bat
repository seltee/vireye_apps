..\converter.exe 0xffff 0x0000 -pw4 tiles1.png -pw4 tiles2.png -pw2 boat.png
clang++ --target=thumb -mthumb -mfloat-abi=soft -fdeclspec -c -m32 -Os effects.cpp comp_math.c pallete.c
..\linker effects.o comp_math.o pallete.o effects.vex
copy effects.vex effects.bin
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -P effects.bin 0x08006000
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -Rst
pause