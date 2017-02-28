//
//  instns.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#include "instns.h"
#include "services.h"
#include "regs.h"
#include "memory.h"
#include "display.h"
#include "keyboard.h"
#include <stdlib.h>

INSTN(screenclr)
    screenclear();
ENDINST

INSTN(ret)
    set_pc(stack_pop16());
ENDINST

INSTN(jp)
    set_pc(GET_ADDR_MASK(instn));
    DEBUGPRINT(printf("Instn: Executing jmp. NPC: 0x%x\n", get_pc()););
    return;
ENDINST

INSTN(call)
    stack_push16(get_pc());
    set_pc(GET_ADDR_MASK(instn));
ENDINST

INSTN(se)
    uint8_t reg_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t param = GET_PARAMS(instn);
    gp_reg_access(regno, &reg_val, REG_LD);
    if (reg_val == param) {
        increment_pc();
    }
ENDINST

INSTN(sne)
    uint8_t reg_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t param = GET_PARAMS(instn);
    gp_reg_access(regno, &reg_val, REG_LD);
    if (reg_val != param) {
        increment_pc();
    }
ENDINST

INSTN(se_reg)
    uint8_t reg_val;
    uint8_t reg2_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg2no = GET_REG2_NO(instn);

    gp_reg_access(regno, &reg_val, REG_LD);
    gp_reg_access(reg2no, &reg2_val, REG_LD);
    if (reg_val == reg2_val) {
        increment_pc();
    }
ENDINST

INSTN(ld)
    uint8_t regno = GET_REG_NO(instn);
    uint8_t param = GET_PARAMS(instn);
    gp_reg_access(regno, &param, REG_ST);
ENDINST

INSTN(add)
    uint8_t regval;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t param = GET_PARAMS(instn);
    uint16_t add_sum;
    gp_reg_access(regno, &regval, REG_LD);
    add_sum = regval + param;
    regval = add_sum;
    if (add_sum > 0xff) DEBUGPRINT(printf("CODE: Overflow for add! PC: 0x%x\n", get_pc() - 2););
    gp_reg_access(regno, &regval, REG_ST);
ENDINST

INSTN(ld_rr)
    uint8_t reg_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg2no = GET_REG2_NO(instn);

    gp_reg_access(reg2no, &reg_val, REG_LD);
    gp_reg_access(regno, &reg_val, REG_ST);
ENDINST

INSTN(or_rr)
    uint8_t reg_val;
    uint8_t reg2_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg2no = GET_REG2_NO(instn);
    gp_reg_access(regno, &reg_val, REG_LD);
    gp_reg_access(reg2no, &reg2_val, REG_LD);
    reg_val |= reg2_val;
    gp_reg_access(regno, &reg_val, REG_ST);
ENDINST

INSTN(and_rr)
    uint8_t reg_val;
    uint8_t reg2_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg2no = GET_REG2_NO(instn);
    gp_reg_access(regno, &reg_val, REG_LD);
    gp_reg_access(reg2no, &reg2_val, REG_LD);
    reg_val &= reg2_val;
    gp_reg_access(regno, &reg_val, REG_ST);
ENDINST

INSTN(xor_rr)
    uint8_t reg_val;
    uint8_t reg2_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg2no = GET_REG2_NO(instn);
    gp_reg_access(regno, &reg_val, REG_LD);
    gp_reg_access(reg2no, &reg2_val, REG_LD);
    reg_val ^= reg2_val;
    gp_reg_access(regno, &reg_val, REG_ST);
ENDINST

INSTN(add_rr)
    toggle_priv(); /* Allow legal access to Vf */
    uint8_t reg_val;
    uint8_t reg2_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg2no = GET_REG2_NO(instn);
    uint16_t add_sum;
    uint8_t overflow = 1;
    gp_reg_access(regno, &reg_val, REG_LD);
    gp_reg_access(reg2no, &reg2_val, REG_LD);
    add_sum = reg_val + reg2_val;
    reg_val = add_sum;
    if (add_sum > 0xff) gp_reg_access(0xf, &overflow, REG_ST);
    gp_reg_access(regno, &reg_val, REG_ST);
    toggle_priv();  /* Reset access priviliges to Vf */
ENDINST

INSTN(sub_rr)
    toggle_priv(); /* Allow legal access to Vf */
    uint8_t reg_val;
    uint8_t reg2_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg2no = GET_REG2_NO(instn);
    uint16_t add_sub;
    uint8_t overflow = 1;
    gp_reg_access(regno, &reg_val, REG_LD);
    gp_reg_access(reg2no, &reg2_val, REG_LD);
    if (reg2_val > reg_val) overflow = 0;
    add_sub = reg_val - reg2_val;
    reg_val = add_sub;
    gp_reg_access(0xf, &overflow, REG_ST);
    gp_reg_access(regno, &reg_val, REG_ST);
    toggle_priv();  /* Reset access priviliges to Vf */
ENDINST

INSTN(shr)
    toggle_priv(); /* Allow legal access to Vf */
    uint8_t reg_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t overflow = 0;
    gp_reg_access(regno, &reg_val, REG_LD);
    if (reg_val % 2) overflow = 1;
    reg_val = reg_val >> 1;
    gp_reg_access(0xf, &overflow, REG_ST);
    gp_reg_access(regno, &reg_val, REG_ST);
    toggle_priv();  /* Reset access priviliges to Vf */
ENDINST

INSTN(subn_rr)
    toggle_priv(); /* Allow legal access to Vf */
    uint8_t reg_val;
    uint8_t reg2_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg2no = GET_REG2_NO(instn);
    uint16_t add_sub;
    uint8_t overflow = 1;
    gp_reg_access(regno, &reg_val, REG_LD);
    gp_reg_access(reg2no, &reg2_val, REG_LD);
    if (reg_val > reg2_val) overflow = 0;
    add_sub = reg2_val - reg_val;
    reg_val = add_sub;
    gp_reg_access(0xf, &overflow, REG_ST);
    gp_reg_access(regno, &reg_val, REG_ST);
    toggle_priv();  /* Reset access priviliges to Vf */
ENDINST

INSTN(shl)
    toggle_priv(); /* Allow legal access to Vf */
    uint8_t reg_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t overflow = 0;
    gp_reg_access(regno, &reg_val, REG_LD);
    if (((int8_t)reg_val) < 0) overflow = 1;
    reg_val = reg_val << 1;
    gp_reg_access(0xf, &overflow, REG_ST);
    gp_reg_access(regno, &reg_val, REG_ST);
    toggle_priv();  /* Reset access priviliges to Vf */
ENDINST

INSTN(sne_reg)
    uint8_t reg_val;
    uint8_t reg2_val;
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg2no = GET_REG2_NO(instn);

    gp_reg_access(regno, &reg_val, REG_LD);
    gp_reg_access(reg2no, &reg2_val, REG_LD);
    if (reg_val != reg2_val) {
        increment_pc();
    }
ENDINST

INSTN(ldi)
    uint16_t param = GET_I_PARAMS(instn);
    I_reg_access(&param, REG_ST);
ENDINST

INSTN(jpv)
    uint16_t param = GET_I_PARAMS(instn);
    uint8_t reg_val;
    gp_reg_access(0, &reg_val, REG_LD);
    param = param + reg_val;
    set_pc(param);
ENDINST

INSTN(rnd)
    uint8_t random = rand();
    uint8_t params = GET_PARAMS(instn);
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg_val = random & params;
    gp_reg_access(regno, &reg_val, REG_LD);
ENDINST

INSTN(sprite)
    uint8_t x_cor;
    uint8_t y_cor;
    uint8_t height = instn & 0xF;
    uint8_t reg_no = GET_REG_NO(instn);
    uint8_t reg2_no = GET_REG2_NO(instn);
    uint8_t overflow = 0;
    uint16_t I_addr;
    uint16_t sprite_data;
    uint32_t row_iterator, pixel_iterator = 0;
    uint8_t pixelVal;
    gp_reg_access(reg_no, &x_cor, REG_LD);
    gp_reg_access(reg2_no, &y_cor, REG_LD);
    I_reg_access(&I_addr, REG_LD);

    /* Loop over each row */
    for (row_iterator = 0; row_iterator < height; row_iterator++) {
        access_memory(I_addr + row_iterator, MEM_LD8, &pixelVal);
        for (pixel_iterator = 0; pixel_iterator < 8; pixel_iterator++) {
            if ((pixelVal & (0x80 >> pixel_iterator)) != 0) {
                /* Need to operate on this pixel */
                get_screen_mutex();
                sprite_data = get_screen()[(x_cor + pixel_iterator + ((y_cor + row_iterator) * TERM_X))];
                if (sprite_data == 1) {
                    /* Pixel already set */
                    overflow = 1;
                }
                sprite_data = sprite_data ^ 1;
                get_screen()[(x_cor + pixel_iterator + ((y_cor + row_iterator) * TERM_X))] = sprite_data;
                release_screen_mutex();
            }
        }
    }
    toggle_priv();
    gp_reg_access(0xF, &overflow, REG_ST);
    toggle_priv();
ENDINST

INSTN(skp)
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg_val;
    gp_reg_access(regno, &reg_val, REG_LD);
    if (check_keyPressed(reg_val)) {
        increment_pc();
    }
ENDINST

INSTN(sknp)
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg_val;
    gp_reg_access(regno, &reg_val, REG_LD);
    if (!check_keyPressed(reg_val)) {
        increment_pc();
    }
ENDINST

INSTN(lddt)
    uint8_t regno = GET_REG_NO(instn);
    uint8_t delay_val;
    delay_timer_reg_access(&delay_val, REG_LD);
    gp_reg_access(regno, &delay_val, REG_ST);
ENDINST

INSTN(ldk)
    uint8_t reg_no = GET_REG_NO(instn);
    int16_t keyPressed;
    uint8_t reg_val;
    while(true) {
        keyPressed = get_keyPressed();
        if (keyPressed != -1) break;
    }
    reg_val = keyPressed;
    gp_reg_access(reg_no, &reg_val, REG_ST);
ENDINST

INSTN(ldvx)
    uint8_t regno = GET_REG_NO(instn);
    uint8_t delay_val;
    gp_reg_access(regno, &delay_val, REG_LD);
    delay_timer_reg_access(&delay_val, REG_ST);
ENDINST

INSTN(ldst)
    uint8_t regno = GET_REG_NO(instn);
    uint8_t delay_val;
    gp_reg_access(regno, &delay_val, REG_LD);
    sound_timer_reg_access(&delay_val, REG_ST);
ENDINST

INSTN(addi)
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg_val;
    uint16_t current_I;
    gp_reg_access(regno, &reg_val, REG_LD);
    I_reg_access(&current_I, REG_LD);
    current_I = current_I + reg_val;
    I_reg_access(&current_I, REG_ST);
ENDINST

INSTN(sprite_load)
    uint8_t regno = GET_REG_NO(instn);
    uint8_t regval;
    uint16_t newI = 0;
    gp_reg_access(regno, &regval, REG_LD);
    if (regval > 0xF) newI = 0;
    else newI = (regval * BYTES_PER_SPRITE);
    I_reg_access(&newI, REG_ST);
ENDINST

INSTN(bcd)
    uint8_t regno = GET_REG_NO(instn);
    uint8_t reg_val;
    uint16_t I_val;
    uint8_t hund, tens, ones;
    gp_reg_access(regno, &reg_val, REG_LD);
    I_reg_access(&I_val, REG_LD);
    hund = reg_val / 100;
    tens = (reg_val / 10) % 10;
    ones = reg_val % 10;
    access_memory(I_val++, MEM_ST8, &hund);
    access_memory(I_val++, MEM_ST8, &tens);
    access_memory(I_val, MEM_ST8, &ones);
ENDINST

INSTN(str)
    uint8_t regno = 0;
    uint8_t regmax = GET_REG_NO(instn);
    uint8_t reg_val;
    uint16_t I_val;
    I_reg_access(&I_val, REG_LD);
    for (regno = 0; regno <= regmax; regno++) {
        gp_reg_access(regno, &reg_val, REG_LD);
        access_memory(I_val++, MEM_ST8, &reg_val);
    }
    I_reg_access(&I_val, REG_ST);
ENDINST

INSTN(ldr)
    uint8_t regno = 0;
    uint8_t regmax = GET_REG_NO(instn);
    uint8_t reg_val;
    uint16_t I_val;
    I_reg_access(&I_val, REG_LD);
    for (regno = 0; regno <= regmax; regno++) {
        access_memory(I_val++, MEM_LD8, &reg_val);
        gp_reg_access(regno, &reg_val, REG_ST);
    }
    I_reg_access(&I_val, REG_ST);
ENDINST
