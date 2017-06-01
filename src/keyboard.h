//
//  keyboard.h
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/23/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifndef keyboard_h
#define keyboard_h

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#define NUM_KEYS    16

void keyboard_init();
bool check_keyPressed(uint8_t keyId);
int16_t get_keyPressed();
void gl_keyDownHandler(unsigned char Key, int one, int two);
void gl_keyUpHandler(unsigned char keyid, int one, int two);


#endif /* keyboard_h */
