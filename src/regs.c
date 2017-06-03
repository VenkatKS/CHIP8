//
//  regs.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#include "regs.h"
#include "hostmem.h"
#include "services.h"
#include "memory.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

gp_regs* active_gp;
I_reg* active_I;
timer_regs* active_timers;
system_regs* active_system;

bool initalized = false;
bool priviliged = false;

pthread_t timer_tid;

clock_t time1 = 0;
clock_t time2 = 0;

void decrement_timers()
{
    if (active_timers->delay_timer > 0) active_timers->delay_timer--;
    if (active_timers->sound_timer > 0) active_timers->sound_timer--;
}

void getTimerLock()
{
    pthread_mutex_lock(&active_timers->count_mutex);
}

void releaseTimerLock()
{
    pthread_mutex_unlock(&active_timers->count_mutex);
}

void* timer_thread()
{
    struct timespec time_duration;
    /* Initialize timers to 60 Hz */
    time_duration.tv_sec = 0;
    time_duration.tv_nsec = 1000000000;

    while(1)
    {
        getTimerLock();
        decrement_timers();
        nanosleep(&time_duration,NULL);
        releaseTimerLock();
    }

}

void regs_init()
{
    active_gp = allocate(sizeof(gp_regs));
    active_I = allocate(sizeof(I_reg));
    active_timers = allocate(sizeof(timer_regs));
    active_system = allocate(sizeof(system_regs));
    active_system->pc = STARTING_PC;
    initialize_sp(&active_system->sp);
    pthread_mutex_init(&active_timers->count_mutex, NULL);
    initalized = true;

    if (pthread_create(&timer_tid, NULL, &timer_thread, NULL) != 0) {
        mypanic("Cannot create timer thread!\n");
        return;
    }

}

void gp_reg_access(uint8_t regno, uint8_t* reg_val, reg_op op) {
    if (!priviliged && regno == 0xf) ERRORPRINT(printf("Illegal use of Vf\n"););
    switch (op) {
        case REG_LD:
            *reg_val = active_gp->Vx[regno];
            ACCESSPRINT(printf("Loading %d from reg %d\n", *reg_val, regno););
            break;
        case REG_ST:
            active_gp->Vx[regno] = *reg_val;
            ACCESSPRINT(printf("Storing %d to reg %d\n", *reg_val, regno););
            break;
        default:
            mypanic("Invalid reg op!\n");
            break;
    }
}

void I_reg_access(uint16_t* reg_val, reg_op op) {
    switch (op) {
        case REG_LD:
            *reg_val = active_I->I;
            ACCESSPRINT(printf("Loading %d from I reg\n", *reg_val););
            break;
        case REG_ST:
            active_I->I = *reg_val;
            ACCESSPRINT(printf("Storing %d to I reg\n", *reg_val););
            break;
        default:
            mypanic("Invalid reg op!\n");
            break;
    }
}

void delay_timer_reg_access(uint8_t* reg_val, reg_op op)
{
    getTimerLock();
    switch (op) {
        case REG_LD:
            *reg_val = active_timers->delay_timer;
            ACCESSPRINT(printf("Loading %d from delay timer reg\n", *reg_val););
            break;
        case REG_ST:
            active_timers->delay_timer = *reg_val;
            ACCESSPRINT(printf("Storing %d to delay timer reg\n", *reg_val););
            break;
        default:
            mypanic("Invalid reg op!\n");
            break;
    }
    releaseTimerLock();
}

void sound_timer_reg_access(uint8_t* reg_val, reg_op op)
{
    getTimerLock();
    switch (op) {
        case REG_LD:
            *reg_val = active_timers->sound_timer;
            ACCESSPRINT(printf("Loading %d from sound timer reg\n", *reg_val););
            break;
        case REG_ST:
            active_timers->sound_timer = *reg_val;
            ACCESSPRINT(printf("Storing %d to sound timer reg\n", *reg_val););
            break;
        default:
            mypanic("Invalid reg op!\n");
            break;
    }
    releaseTimerLock();
}

void increment_pc()
{
    MYASSERT(initalized);
    active_system->pc+=2;
}

void set_pc(uint16_t npc)
{
    MYASSERT(!(npc & 0xF000))
    active_system->pc = npc;
}

uint16_t get_pc()
{
    return active_system->pc;
}

void set_sp(uint16_t* sp_loc)
{
    active_system->sp = sp_loc;
}

void increment_sp()
{
    if (!check_stack(active_system->sp)) {
        ERRORPRINT(printf("Error: The Stack Has Overflowed! SP: 0x%x\n", (uint32_t) active_system->sp););
        mypanic("Stack overflow!\n");
    }
    active_system->sp++;
}

void decrement_sp()
{
    active_system->sp--;
}

uint16_t* get_sp()
{
    return active_system->sp;
}

void stack_push16(uint16_t data)
{
    increment_sp();
    *active_system->sp = data;
}

uint16_t stack_pop16()
{
    uint16_t returnData = *active_system->sp;
    decrement_sp();
    return returnData;
}

void toggle_priv()
{
    priviliged = !priviliged;
}
