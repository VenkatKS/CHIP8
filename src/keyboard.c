//
//  keyboard.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/23/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#include "keyboard.h"
#include "services.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t keyboard_fifo_mutex;

bool	key_status[NUM_KEYS];

/* Keyboard FIFO Operations */
#define GET_KEYBOARD_MUTEX								\
	pthread_mutex_lock(&keyboard_fifo_mutex);

#define RELEASE_KEYBOARD_MUTEX								\
	pthread_mutex_unlock(&keyboard_fifo_mutex);

/* Default Keybindings */
key_binding ACTIVE_BINDING;

void keyboard_init()
{
	pthread_mutex_init(&keyboard_fifo_mutex, NULL);
	memset(key_status, false, NUM_KEYS);
	memset(ACTIVE_BINDING.keycodes, -1, NUM_ASCII);

	/* Initialize default keycodes */
	ACTIVE_BINDING.keycodes['4'] = 0;
	ACTIVE_BINDING.keycodes['1'] = 1;
	ACTIVE_BINDING.keycodes['2'] = 2;
	ACTIVE_BINDING.keycodes['3'] = 3;
	ACTIVE_BINDING.keycodes['q'] = 4;
	ACTIVE_BINDING.keycodes['w'] = 5;
	ACTIVE_BINDING.keycodes['e'] = 6;
	ACTIVE_BINDING.keycodes['a'] = 7;
	ACTIVE_BINDING.keycodes['s'] = 8;
	ACTIVE_BINDING.keycodes['d'] = 9;
	ACTIVE_BINDING.keycodes['z'] = 0xA;
	ACTIVE_BINDING.keycodes['x'] = 0xB;
	ACTIVE_BINDING.keycodes['c'] = 0xC;
	ACTIVE_BINDING.keycodes['b'] = 0xD;
	ACTIVE_BINDING.keycodes['n'] = 0xE;
	ACTIVE_BINDING.keycodes['m'] = 0xF;
}

/*
 * A downside of our keybinding implementation is the increased
 * time it takes to actually unbind a key. But it's still better
 * than having to iterate the array everytime a key is pressed.
 */

bool resetKeyBinding(uint8_t chip8key, uint8_t ascii) {
	int idx = 0;

	/* Not an ascii we can tolerate */
	if (ascii >= NUM_ASCII) return false;

	/* Remove old key binding */
	for (idx = 0; idx < NUM_ASCII; idx++) {
		if (ACTIVE_BINDING.keycodes[idx] == chip8key) {
			ACTIVE_BINDING.keycodes[idx] = -1;
			break;
		}
	}

	/* Set the new key binding */
	ACTIVE_BINDING.keycodes[ascii] = chip8key;
	return true;
}


#define IS_CAPITAL_LETTER(_ascii)	(_ascii >= 'A' && _ascii <= 'Z')
#define CAPITAL_OFFSET			0x20

int16_t get_keyMap(int keyCharacter)
{
	int keyCh = -1;

	/* Convert into lower case */
	if (IS_CAPITAL_LETTER(keyCharacter))
		keyCh = keyCharacter + CAPITAL_OFFSET;
	else
		keyCh = keyCharacter;

	return ACTIVE_BINDING.keycodes[keyCh];
}

bool check_keyPressed(uint8_t keyId)
{
	bool is_pressed = false;
	if (keyId >= NUM_KEYS) return false;
	GET_KEYBOARD_MUTEX;
	is_pressed = (key_status[keyId]);
	RELEASE_KEYBOARD_MUTEX;
	return is_pressed;
}

int16_t get_keyPressed()
{
	int16_t val_pressed = -1;
	GET_KEYBOARD_MUTEX;
	for (int i = 0; i < NUM_KEYS; i++) {
		if (key_status[i]) val_pressed = i;
	}
	RELEASE_KEYBOARD_MUTEX;
	return val_pressed;

}

void gl_keyUpHandler(unsigned char keyid, int one, int two) {
	int16_t key_idx = get_keyMap(keyid);
	if (key_idx == -1) return;
	GET_KEYBOARD_MUTEX;
	key_status[key_idx] = false;
	RELEASE_KEYBOARD_MUTEX;
}

void gl_keyDownHandler(unsigned char keyid, int one, int two)
{
	int16_t key_idx = get_keyMap(keyid);
	if (key_idx == -1) return;
	GET_KEYBOARD_MUTEX;
	key_status[key_idx] = true;
	RELEASE_KEYBOARD_MUTEX;
}
