clang++ --target=thumb -mthumb -mfloat-abi=soft -fdeclspec -c -m32 -Os dynamic_memory.cpp
..\linker dynamic_memory.o dynamic_memory.vex
copy dynamic_memory.vex dynamic_memory.bin
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -P dynamic_memory.bin 0x08006000
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -Rst
pause