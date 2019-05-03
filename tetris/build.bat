clang++ --target=thumb -mthumb -mfloat-abi=soft -fdeclspec -c -m32 -Os tetris.cpp field.cpp highScore.cpp sound.cpp
..\linker tetris.o field.o highScore.o sound.o tetris.vex
..\patcher ..\vireye_core.hex tetris.vex tetris.hex
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -P tetris.hex 0x08000000
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -Rst
pause