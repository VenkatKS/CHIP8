//
//  decoder.h
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifndef decoder_h
#define decoder_h

#include <stdio.h>
#include <inttypes.h>

#define GET_TOP_NIBBLE(two_bytes) ((two_bytes & 0xF000) >> 12)
#define GET_BOTTOM_NIBBLE(two_bytes) (two_bytes & 0xF)
#define GET_SECOND_NIBBLE(two_bytes) (two_bytes & 0xF0) >> 4
/* Main processor emulation loop */
void* run_me();
void start_decoder();
void stop_decoder();
void stop_decoder();


#endif /* decoder_h */
