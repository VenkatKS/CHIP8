//
//  regs.h
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifndef regs_h
#define regs_h

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <inttypes.h>

typedef struct {
    uint8_t Vx[16];
} gp_regs;

typedef struct {
  uint16_t I;  
} I_reg;

typedef struct {
    uint8_t delay_timer;
    uint8_t sound_timer;
    pthread_mutex_t count_mutex;
} timer_regs;

typedef struct {
    uint16_t pc;
    uint16_t* sp;
} system_regs;

typedef enum {
    REG_LD,
    REG_ST
} reg_op;

void regs_init();


/* PROGRAM COUNTER */
uint16_t get_pc();
void set_pc(uint16_t npc);
void increment_pc();

/* STACK POINTER */
void set_sp(uint16_t* sp_loc);
void stack_push16(uint16_t data);
uint16_t stack_pop16();
uint16_t* get_sp();

/* GENERAL PURPOSE REGISTERS */
void gp_reg_access(uint8_t regno, uint8_t* reg_val, reg_op op);

/* I REGISTER */
void I_reg_access(uint16_t* reg_val, reg_op op);

/* TIMER REGISTERS */
void delay_timer_reg_access(uint8_t* reg_val, reg_op op);
void sound_timer_reg_access(uint8_t* reg_val, reg_op op);

void toggle_priv();

#endif /* regs_h */
