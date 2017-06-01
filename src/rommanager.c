//
//  rommanager.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/24/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#include "rommanager.h"
#include "services.h"
#include "hostmem.h"
#include "memory.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#define USER_MEMORY_SZ (4096 - 512)

void rom_init(const char* name)
{
    ACCESSPRINT(printf("Loading ROM File %s:\n", name););

    FILE* rom_file = fopen(name, "rb");
    uint8_t* rom_buffer;
    uint64_t rom_file_sz;
    size_t buffer_valid_sz;
    uint32_t buffer_iterator = 0;

    if (rom_file == NULL) {
        mypanic("Illegal ROM file.\n");
        return;
    }

    fseek(rom_file , 0 , SEEK_END);
    rom_file_sz = ftell(rom_file);
    rewind(rom_file);

    if (rom_file_sz > USER_MEMORY_SZ) {
        ERRORPRINT(printf("ROM file too big! Useable memory: %d bytes, ROM size: %" PRIx64 " bytes", USER_MEMORY_SZ, rom_file_sz););
        mypanic("ROM file too big!");
    }

    rom_buffer = (uint8_t *)allocate((uint32_t) rom_file_sz);

    buffer_valid_sz = fread (rom_buffer, 1, rom_file_sz, rom_file);
    if (buffer_valid_sz != rom_file_sz)
    {
        mypanic("Could not read entire file!");
    }

    for (buffer_iterator = 0; buffer_iterator < rom_file_sz; buffer_iterator++) {
        access_memory(512 + buffer_iterator, MEM_ST8, &rom_buffer[buffer_iterator]);
    }

    freeAlloc(rom_buffer);
    fclose(rom_file);

    return;
    
}
