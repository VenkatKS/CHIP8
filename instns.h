//
//  instns.h
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifndef instns_h
#define instns_h

#include <stdio.h>
#include <inttypes.h>

#define INSTN(_name)            void func_##_name(uint16_t instn) {
#define ENDINST                 }
#define INSTN_DEF(_name)        void func_##_name(uint16_t instn);
#define EXECUTE_INSTN(_name)    func_##_name(instn);

/* MASKS */
#define ADDR_MASK               0x0FFF
#define GET_ADDR_MASK(_addr)    (_addr & ADDR_MASK)

#define REG_MASK                0x0F00
#define REG_SHIFT               8
#define GET_REG_NO(_addr)       ((_addr & REG_MASK) >> REG_SHIFT)

#define REG2_MASK               0x00F0
#define REG2_SHIFT              4
#define GET_REG2_NO(_addr)      (((_addr) & REG2_MASK) >> REG2_SHIFT)

#define PARAMS_MASK             0xFF
#define GET_PARAMS(_addr)       (_addr & PARAMS_MASK)

#define I_PARAMS_MASK           0xFFF
#define GET_I_PARAMS(_addr)     (_addr & I_PARAMS_MASK)

INSTN_DEF(screenclr);   /* 0x00E0 CLR */
INSTN_DEF(ret);         /* 0x00EE RET */
INSTN_DEF(jp);          /* 0x1XXX JP */
INSTN_DEF(call);        /* 0x2XXX CALL */
INSTN_DEF(se);          /* 0x3XXX SE */
INSTN_DEF(sne);         /* 0x4XXX SE */
INSTN_DEF(se_reg);      /* 0x5XX0 SE */
INSTN_DEF(ld)           /* 0x6XXX LD */
INSTN_DEF(add);         /* 0x7XXX ADD */
INSTN_DEF(ld_rr);       /* 0x8XX0 LD */
INSTN_DEF(or_rr);       /* 0x8XX1 OR */
INSTN_DEF(and_rr);      /* 0x8XX2 AND */
INSTN_DEF(xor_rr);      /* 0x8XX3 XOR */
INSTN_DEF(add_rr);      /* 0x8XX4 ADD */
INSTN_DEF(sub_rr);      /* 0x8XX5 SUB */
INSTN_DEF(shr);         /* 0x8X06 SHR */
INSTN_DEF(subn_rr);     /* 0x8XX7 SUBN */
INSTN_DEF(shl);         /* 0x8XXE SHL */
INSTN_DEF(sne_reg);     /* 0x9XX0 SE */
INSTN_DEF(ldi);         /* 0xAXXX LDI */
INSTN_DEF(jpv);         /* 0xBXXX JPV0 */
INSTN_DEF(rnd);         /* 0xCXXX RND */
INSTN_DEF(sprite);      /* 0xDXXX DRW */
INSTN_DEF(skp);         /* 0xEX9E SKP */
INSTN_DEF(sknp);        /* 0xEXA1 SKNP */
INSTN_DEF(lddt);        /* 0xFX07 LD DT */
INSTN_DEF(ldk);         /* 0xFX0A LD K */
INSTN_DEF(ldvx);        /* 0xFX15 LD VX */
INSTN_DEF(ldst);        /* 0xFX18 LD ST */
INSTN_DEF(addi);        /* 0xFX1E ADD I */
INSTN_DEF(sprite_load); /* 0xFX29 SPR */
INSTN_DEF(bcd);         /* 0xFX33 LD BCD */
INSTN_DEF(str);         /* 0xFX55 STR */
INSTN_DEF(ldr);         /* 0xFX65 LDR */

#endif /* instns_h */
