@echo off
rmdir Yield /s /q
mkdir Yield
rmdir Debug /s /q
mkdir Debug
clang -m32 -Oz -ffunction-sections -fdata-sections -Wl,/OPT:REF,/OPT:ICF,/DEBUG:NONE -fno-unroll-loops -fshort-enums -o "./Yield/CLI_BG.exe" "./Source/Core.c" "./Source/Code39.c" "./Source/Code128.c" "./Source/ITF.c" "./Source/Transcriber.c"
clang -m32 -O0 -g -o "./Debug/CLI_BG.exe" "./Source/Core.c" "./Source/Code39.c" "./Source/Code128.c" "./Source/ITF.c" "./Source/Transcriber.c"