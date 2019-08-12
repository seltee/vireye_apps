clang++ --target=thumb -mthumb -mfloat-abi=soft -fdeclspec -c -m32 -Os tetris.cpp field.cpp highScore.cpp sound.cpp
..\linker tetris.o field.o highScore.o sound.o tetris.vex
copy tetris.vex tetris.bin
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -P tetris.bin 0x08006000
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -Rst
pause