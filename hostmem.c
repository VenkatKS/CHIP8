//
//  hostmem.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#include "hostmem.h"
#include "services.h"
#include <stdint.h>
#include <stdlib.h>


uint64_t numAllocations = 0;

void* allocate(uint32_t bytes)
{
    ACCESSPRINT(printf("Allocating %d bytes...\n", bytes););
    void* returnPointer = malloc(bytes);
    if (!returnPointer) {
        mypanic("Cannot malloc anymore memory! Location: hostmem.c:\n");
    }
    numAllocations++;
    return returnPointer;
}

void freeAlloc(void* pointer)
{
    ACCESSPRINT(printf("Freeing...\n"););
    numAllocations--;
    free(pointer);
}

