# CHIP8
Full instruction-level emulator for the CHIP-8 Instruction Set Architecture (ISA). This emulator uses OPENGL for the actual CHIP8 graphics support and for other functional GUI features (load new ROM, etc.).

## How to Build
To build, please run make from src/.

## How to Run
The emulator automatically scans its directory and all sub-directories (and sub-sub-directories, etc.) for CHIP8 ROM binaries (any file with .c8 extension). Once the emulator is launched, please click anywhere and hover over "Load ROM" menu option. All your ROMs should be populated there. If you can't find your binary there, please ensure the file you want to run has the extension ".c8" and is in the same folder (or its subfolders) as the emulator. Some sample roms can be found under the src/build/sample_roms folder in this repository.

## Screenshots
![Breakout](https://github.com/VenkatKS/CHIP8/raw/master/screenshots/breakout.png "Breakout Game")
![Breakout](https://github.com/VenkatKS/CHIP8/raw/master/screenshots/invaders.png "Invaders Game")


## Capabilites
CHIP8 allows the user to:
* Run all CHIP8 ROMs (full support for the entire ISA)
* Quickly launch a new ROM (in the same directory tree) by navigating a menu
* Re-bind keys through a guided graphical interface
* Pause/Play emulation


Developed, compiled, and tested on MacOS.

