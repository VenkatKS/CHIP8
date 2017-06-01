//
//  memory.h
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifndef memory_h
#define memory_h

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#define FOUR_KB     4096
#define STACK_WORDS 16

typedef enum {
    MEM_LD8,
    MEM_ST8,
    MEM_LD16,
    MEM_ST16
} mem_access_type;

void init_memory();
void initialize_sp(uint16_t** sp);
bool check_stack(uint16_t* sp);

bool access_memory(uint16_t addr, mem_access_type mem_op, void* value);
#endif /* memory_h */
