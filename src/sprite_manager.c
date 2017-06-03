//
//  display.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//
/* OpenGL Libraries */
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include "services.h"
#include "memory.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "sprite_manager.h"

int magnification = 10;


uint8_t chip8_fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/* NOTE: Ensure you have mutex before calling! */

void sprite_init()
{
	uint16_t xIterator, yIterator, memoryIterator = 0;
	ACCESSPRINT(printf("Loading digit sprites into memory.\n"););
	for (xIterator = 0; xIterator < NUM_SPRITES; xIterator++) {
		for (yIterator = 0; yIterator < BYTES_PER_SPRITE; yIterator++) {
			access_memory(memoryIterator++, MEM_ST8, &chip8_fontset[(xIterator * BYTES_PER_SPRITE) + yIterator]);
		}
	}
	ACCESSPRINT(printf("Finished loading digit sprites into memory.\n"););
}

