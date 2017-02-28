//
//  decoder.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#include "decoder.h"
#include "services.h"
#include "memory.h"
#include "regs.h"
#include "instns.h"
#include "display.h"
#include "keyboard.h"
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

bool screenBufferChanged = false;

void decode_me(uint16_t instn)
{
    uint8_t topnibble = GET_TOP_NIBBLE(instn);
    uint8_t bottomnibble = GET_BOTTOM_NIBBLE(instn);
    uint8_t secondnibble = GET_SECOND_NIBBLE(instn);
    switch (topnibble) {
        case 0x0:
            switch (bottomnibble) {
                case 0x0:
                    EXECUTE_INSTN(screenclr);
                    break;
                case 0xE:
                    EXECUTE_INSTN(ret);
                    break;
                default:
                    mypanic("Illegal 0x0 instruction variation.\n");
                    break;
            }
            break;
        case 0x1:
            /* JP Addr */
            EXECUTE_INSTN(jp);
            break;
        case 0x2:
            EXECUTE_INSTN(call);
            break;
        case 0x3:
            EXECUTE_INSTN(se);
            break;
        case 0x4:
            EXECUTE_INSTN(sne);
            break;
        case 0x5:
            MYASSERT(!bottomnibble)
            EXECUTE_INSTN(se_reg);
            break;
        case 0x6:
            EXECUTE_INSTN(ld);
            break;
        case 0x7:
            EXECUTE_INSTN(add);
            break;
        case 0x8:
            switch (bottomnibble) {
                case 0x0:
                    EXECUTE_INSTN(ld_rr);
                    break;
                case 0x1:
                    EXECUTE_INSTN(or_rr);
                    break;
                case 0x2:
                    EXECUTE_INSTN(and_rr);
                    break;
                case 0x3:
                    EXECUTE_INSTN(xor_rr);
                    break;
                case 0x4:
                    EXECUTE_INSTN(add_rr);
                    break;
                case 0x5:
                    EXECUTE_INSTN(sub_rr);
                    break;
                case 0x6:
                    EXECUTE_INSTN(shr);
                    break;
                case 0x7:
                    EXECUTE_INSTN(subn_rr);
                    break;
                case 0xE:
                    EXECUTE_INSTN(shl);
                    break;
                default:
                    mypanic("Illegal 0x8 instruction variation.\n");
                    break;
            }
            break;
        case 0x9:
            MYASSERT(!bottomnibble)
            EXECUTE_INSTN(sne_reg);
            break;
        case 0xA:
            EXECUTE_INSTN(ldi);
            break;
        case 0xB:
            EXECUTE_INSTN(jpv);
            break;
        case 0xC:
            EXECUTE_INSTN(rnd);
            break;
        case 0xD:
            EXECUTE_INSTN(sprite);
            screenBufferChanged = true;
            break;
        case 0xE:
            switch (bottomnibble) {
                case 0x1:
                    MYASSERT((secondnibble == 0xA));
                    EXECUTE_INSTN(sknp);
                    break;
                case 0xE:
                    MYASSERT((secondnibble == 0x9));
                    EXECUTE_INSTN(skp);
                    break;
                default:
                    mypanic("Illegal 0xE instruction variation.\n");
                    break;
            }
            break;
        case 0xF:
            switch (bottomnibble) {
                case 0x3:
                    MYASSERT((secondnibble == 0x3));
                    EXECUTE_INSTN(bcd);
                    break;
                case 0x5:
                    switch (secondnibble) {
                        case 0x1:
                            EXECUTE_INSTN(ldvx);
                            break;
                        case 0x5:
                            EXECUTE_INSTN(str);
                            break;
                        case 0x6:
                            EXECUTE_INSTN(ldr);
                            break;
                        default:
                            mypanic("Illegal 0xEX51 instruction.\n");
                            break;
                    }
                    break;
                case 0x7:
                    MYASSERT((secondnibble == 0x0));
                    EXECUTE_INSTN(lddt);
                    break;
                case 0x8:
                    MYASSERT((secondnibble == 0x1));
                    EXECUTE_INSTN(ldst);
                    break;
                case 0x9:
                    MYASSERT((secondnibble == 0x2));
                    EXECUTE_INSTN(sprite_load);
                    break;
                case 0xA:
                    MYASSERT((secondnibble == 0x0));
                    EXECUTE_INSTN(ldk);
                    break;
                case 0xE:
                    MYASSERT((secondnibble == 0x1));
                    EXECUTE_INSTN(addi);
                    break;
                default:
                    mypanic("Illegal 0xF instruction variation.\n");
                    break;
            }
            break;
        default:
            mypanic("Incorrectly parsed instruction.\n");
            break;
    }
}

pthread_t display_tid;
void run_me()
{
    uint16_t nextInstruction = 0;

    struct timespec time_duration;
    time_duration.tv_sec = 0;
    time_duration.tv_nsec = 1000000;

#if !defined(MASTER_DEBUG)
    if (pthread_create(&display_tid, NULL, &screen_display, NULL) != 0) {
        mypanic("Cannot create timer thread!\n");
        return;
    }
#endif

    while (true) {
        /* Constantly run the processor */
        /* Fetch */
        access_memory(get_pc(), MEM_LD16, &nextInstruction);
        increment_pc();
        
        /* Decode/Execute */
        decode_me(nextInstruction);

        nanosleep(&time_duration, NULL);
    }
}


