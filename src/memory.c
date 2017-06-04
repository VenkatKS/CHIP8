//
//  memory.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#include "memory.h"
#include "hostmem.h"
#include "services.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

uint8_t memory[FOUR_KB];
uint16_t stack_space[STACK_WORDS];

void init_memory()
{
    memset(memory, 0, FOUR_KB);
    memset(stack_space, 0, STACK_WORDS * 2);
}

bool access_memory(uint16_t addr, mem_access_type mem_op, void* value)
{
    /* MMU Protection Code */
    if (addr > 0xFFF) {
        ERRORPRINT(printf("Illegal access to memory. Addr: 0x%x\n", addr););
        mypanic("System accessed illegal portion of memory.\n");
        return false;
    }

    switch (mem_op)
    {
        case MEM_LD8:
            *(uint8_t*)value = (memory[addr]);
            ACCESSPRINT(printf("Access: 8 bit load from 0x%x, rec'v value 0x%x...\n", addr, *(uint8_t*)value););
            break;
        case MEM_LD16:
            *(uint16_t*)value = (memory[addr] << 8) | (memory[addr + 1]);
            ACCESSPRINT(printf("Access: 16 bit load from 0x%x, rec'v value 0x%x...\n", addr, *(uint16_t*)value););
            break;
        case MEM_ST8:
            memory[addr] = *(uint8_t*)value;
            ACCESSPRINT(printf("Access: 8 bit store to 0x%x, stored value 0x%x...\n", addr, *(uint8_t*)value););
            break;
        case MEM_ST16:
            memory[addr + 1] = (*(uint8_t*) value);
            memory[addr] = ((*(uint16_t*)value) & 0xFF00) >> 8;
            ACCESSPRINT(printf("Access: 16 bit load from 0x%x, rec'v value 0x%x...\n", addr, *(uint16_t*)value););
            break;
        default:
            ERRORPRINT(printf("Error: Invalid mem_op...\n"););
            return (false);
    }
    return (true);
}

void initialize_sp(uint16_t** spp)
{
    *(spp) = &(stack_space[0]);
    (*spp)--;
}

/* Returns true if the stack still has space to grow */
bool check_stack(uint16_t* sp)
{
    if (sp == &(stack_space[STACK_WORDS - 1])) {
        return false;
    }
    return true;
}
