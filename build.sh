#!/bin/bash
rm -rf yield
mkdir yield
rm -rf debug
mkdir debug
#clang -m32 -Oz -ffunction-sections -fdata-sections -Wl,/OPT:REF,/OPT:ICF,/DEBUG:NONE -fno-unroll-loops -fshort-enums -o "./Yield/CLI_BG" "./source/Core.c" "./source/Code39.c" "./source/Code128.c" "./source/ITF.c" "./source/Transcriber.c"
clang -m32 -O0 -std=c23 -g -o "./debug/cli_bg" "./source/core.c" "./source/code39.c" "./source/code128.c" "./source/itf.c" "./source/datamatrix.c" "./source/transcriber.c"