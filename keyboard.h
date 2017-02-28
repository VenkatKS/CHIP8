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
/* Returns true if a new key press is recorded, false if nothing */
bool check_keyPressed(uint8_t keyId);
/* Returns ID of the lowest valued key currently pressed */
int16_t get_keyPressed();

#endif /* keyboard_h */
