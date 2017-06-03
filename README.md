# CHIP8
Full instruction-level emulator for the CHIP-8 Instruction Set Architecture (ISA)

Code features an emulator that fully supports all instructions featured in the CHIP-8 ISA.
The code in the main branch uses OPENGL libraries to display the CHIP8 graphics output. The code in the terminal branch uses a Unix terminal for graphics output, and does not use any external graphics libraries.

## How to Build
To build, please run make from src/.

## How to Run
The emulator automatically scans its directory and all sub-directories (and sub-sub-directories, etc.) for CHIP8 ROM binaries (any file with .c8 extension). Once the emulator is launched, please click anywhere and hover over "Load ROM" menu option. All your ROMs should be populated there. If you can't find your binary there, please ensure the file you want to run has the extension ".c8" and is in the same folder (or its subfolders) as the emulator.

Developed, compiled, and tested on MacOS.

