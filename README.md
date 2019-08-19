## What this, OwO???
This repository contains different examples and basic programs for VirEye homebrew game console. 
Visit main repo for more information: https://github.com/seltee/vireye_core

## Runner
This is basic SD file manager (Also it can play .wav files!). If you put autorun.vex after build to the root of your SD card, console will start it after boot every time.
Press select to configure your game console.

## Sound check
This little program is intended to check are all of your sound DAC pins soldered correctly. Use A and B buttons to switch pins. Helpfull only if you are using resistor built DAC.

## Tetris
Brick Game/Tetris made for Vireye. Includes level changing, different figures, simple sound and memorizing high score throw SD card. Basic game example.

## Dynamic Memory
This program shows you the basics of new and delete and also one direction lists. Note, that memory allocates by 16 byte blocks, so, if you're allocating 4 bytes, you will get 16 byte block from memory.

## Converting sprites
It's an example of how you can easily convert images into c arrays suitable for running on vireye and how to use this arrays to show real sprites on your game console. More info about flags and how to use converter you may found on tools repo: https://github.com/seltee/vireye_tools

## Effects
This example shows how to use pre and post effects functions. "setPreProcessCallback" and "setPostProcessCallback" gets functions, that will be called on each string of pixels. First one is called before engine will render sprites and the second one calls when sprites is rendered into the pixel string. Note, that you working here with raw RGB565 data. And the usual mask for it is rrrrrggggggbbbbb. But display needs reversed bytes of color data, so, your mask will be gggbbbbbrrrrrggg that must be sent to display. Where left green bits is actualy start of color description.