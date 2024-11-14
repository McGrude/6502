#!/bin/sh

make && ./bin/emulator.out -L 0x8000:example.bin -L 0xE000:rom.bin

