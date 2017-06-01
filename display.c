//
//  display.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#include "services.h"
#include "memory.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "display.h"

pthread_mutex_t display_mutex;
int magnification = 10;

bool new_frame = false;

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

uint8_t screen[TERM_X * TERM_Y];

uint8_t* get_screen()
{
	return screen;
}

void get_screen_mutex()
{
	pthread_mutex_lock(&display_mutex);
}

void release_screen_mutex()
{
	pthread_mutex_unlock(&display_mutex);
}

/* NOTE: Ensure you have mutex before calling! */
void raise_frame()
{
	new_frame = true;
}

void lower_frame()
{
	new_frame = false;
}

bool check_frame()
{
	return new_frame;
}

void screenclear()
{
	get_screen_mutex();
	memset(screen, 0, TERM_X * TERM_Y);
	release_screen_mutex();
}

#define SET_PIXEL_AT(_i, _j, _mag)							\
	glBegin(GL_QUADS);								\
	glVertex3f((_i * _mag) + 0.0f,     (_j * _mag) + 0.0f,	 0.0f);			\
	glVertex3f((_i * _mag) + 0.0f,     (_j * _mag) + magnification, 0.0f);		\
	glVertex3f((_i * _mag) + _mag, (_j * _mag) + _mag, 0.0f);			\
	glVertex3f((_i * _mag) + _mag, (_j * _mag) + 0.0f,	 0.0f);			\
	glEnd();

void draw_screen()
{
	get_screen_mutex();
	/* Need to be fast -- no reason to call debugging allocate func */
	uint8_t *local_screen = (uint8_t *) malloc(TERM_X * TERM_Y);
	memcpy(local_screen, screen, TERM_X * TERM_Y);
	lower_frame();
	release_screen_mutex();
	uint32_t i, j = 0;
	for (i = 0; i < TERM_X; i++) {
		for (j = 0; j < TERM_Y; j++) {
			if (local_screen[i + ((j) * TERM_X)])
				glColor3f(0.0f,0.0f,0.0f);
			else
				glColor3f(1.0f,1.0f,1.0f);
			SET_PIXEL_AT(i, j, magnification);
		}
	}
	free(local_screen);
	glutSwapBuffers();
}

void display_init()
{
	uint16_t xIterator, yIterator, memoryIterator = 0;
	ACCESSPRINT(printf("Loading digit sprites into memory.\n"););
	for (xIterator = 0; xIterator < NUM_SPRITES; xIterator++) {
		for (yIterator = 0; yIterator < BYTES_PER_SPRITE; yIterator++) {
			access_memory(memoryIterator++, MEM_ST8, &chip8_fontset[(xIterator * BYTES_PER_SPRITE) + yIterator]);
		}
	}
	ACCESSPRINT(printf("Finished loading digit sprites into memory.\n"););
	pthread_mutex_init(&display_mutex, NULL);
}

void reshape_window(GLsizei w, GLsizei h)
{
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}


