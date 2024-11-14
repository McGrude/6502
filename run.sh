#!/bin/sh

make && ./bin/emulator.out -L 0x8000:example.bin
