# CHIP8
Full instruction-level emulator for the CHIP-8 Instruction Set Architecture (ISA)

Code features an emulator that fully supports all instructions featured in the CHIP-8 ISA.
The code does not use any external graphics or keyboard libraries. Instead, it uses Unix-specific terminal commands to draw game graphics on the terminal.
Simiarly, it uses Unix-specific terminal commands to read in key strokes from stdin.

To run, please first compile the files *(by running `gcc *.c` from project directory)*. After doing so, the compiled emulator binary can be launched by specifying a CHIP-8 binary/ROM file as the second parameter.

Developed, compiled, and tested on MacOS.

