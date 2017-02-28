//
//  main.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/21/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#include "regs.h"
#include "memory.h"
#include "decoder.h"
#include "display.h"
#include "keyboard.h"
#include "services.h"
#include "rommanager.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {
    if (argc < 2) mypanic("Please specify ROM file!\n");
    init_memory();
    display_init();
    keyboard_init();
    regs_init();
    rom_init(argv[1]);
    run_me();
    return 0;
}
