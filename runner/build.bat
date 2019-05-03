clang++ --target=thumb -mthumb -mfloat-abi=soft -c -m32 -Os -fdeclspec runner.cpp selector.cpp utils.cpp sound_player.cpp config.cpp
..\linker runner.o selector.o utils.o sound_player.o config.o autorun.vex
..\patcher ..\vireye_core.hex autorun.vex runner.hex
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -P runner.hex 0x08000000
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -Rst
pause