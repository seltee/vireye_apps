clang++ --target=thumb -mthumb -mfloat-abi=soft -fdeclspec -c -m32 -Os sound_check.cpp
..\linker sound_check.o sound_check.vex
copy sound_check.vex sound_check.bin
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -P sound_check.bin 0x08006000
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -Rst
pause