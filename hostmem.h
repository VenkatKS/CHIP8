//
//  hostmem.h
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifndef hostmem_h
#define hostmem_h

#include <stdio.h>
#include <inttypes.h>

void* allocate(uint32_t bytes);
void freeAlloc(void* pointer);


#endif /* hostmem_h */
