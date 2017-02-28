//
//  services.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//


#include "services.h"
#include <stdlib.h>

void mypanic(char* errmsg)
{
    printf("PANIC: %s", errmsg);
    exit(-1);
}

void assertionfail()
{
    exit(-1);
}
