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
#define NUM_ASCII   0x7f

/* Key Binding Struct */

/*
 * NOTE ABOUT KEYBINDINGS:
 *	We can implement this in one way or another.
 *	If we minimize memory used, we'll only need
 *	NUM_KEYS byte array. For every key pressed,
 *	we'll need to iterate this small array to get
 *	the actual CHIP8 keycode. Instead, we can expand
 *	the amount of memory we use to all avaliable ascii
 *	codes and use that as a hashtable for the ascii code
 *	for the button pressed to get the CHIP8 keycode. The second
 *	method is MUCH faster than the first. Thus, this
 *	implements the latter for speed.
 */
typedef struct {
	uint8_t keycodes[NUM_ASCII];
} key_binding;

/* Function to change the key binding of a CHIP8 key */
bool resetSpecificKeyBinding(uint8_t chip8key, uint8_t ascii);
void rebind_keys();
void set_key_binding_default();

void keyboard_init();
bool check_keyPressed(uint8_t keyId);
int16_t get_keyPressed();
void gl_keyDownHandler(unsigned char Key, int one, int two);
void gl_keyUpHandler(unsigned char keyid, int one, int two);


#endif /* keyboard_h */
