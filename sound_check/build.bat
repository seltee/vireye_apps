clang++ --target=thumb -mthumb -mfloat-abi=soft -fdeclspec -c -m32 -Os sound_check.cpp
..\linker sound_check.o sound_check.vex
..\patcher ..\vireye_core.hex sound_check.vex sound_check.hex
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -P sound_check.hex 0x08000000
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -Rst
pause