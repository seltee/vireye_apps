clang++ --target=thumb -mthumb -mfloat-abi=soft -c -m32 -Os -fdeclspec runner.cpp selector.cpp utils.cpp sound_player.cpp config.cpp -Wignored-attributes
..\linker runner.o selector.o utils.o sound_player.o config.o autorun.vex
copy autorun.vex runner.bin
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -P runner.bin 0x08006000
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -Rst
pause