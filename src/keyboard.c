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

void keyboard_init()
{
	pthread_mutex_init(&keyboard_fifo_mutex, NULL);
	memset(key_status, false, NUM_KEYS);
}

int16_t get_keyMap(int keyCharacter)
{
	switch (keyCharacter) {
		case '4':
			return (0);
		case '1':
			return (1);
		case '2':
			return (2);
		case '3':
			return (3);
		case 'q':
		case 'Q':
			return (4);
		case 'w':
		case 'W':
			return (5);
		case 'e':
		case 'E':
			return (6);
		case 'a':
		case 'A':
			return (7);
		case 's':
		case 'S':
			return (8);
		case 'd':
		case 'D':
			return (9);
		case 'z':
		case 'Z':
			return (10);
		case 'x':
		case 'X':
			return (11);
		case 'c':
		case 'C':
			return (12);
		case 'b':
		case 'B':
			return (13);
		case 'n':
		case 'N':
			return (14);
		case 'm':
		case 'M':
			return (15);
		default:
			/* Not an actual emulated key */
			return (-1);
			break;
	}
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
