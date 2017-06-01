//
//  display.h
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifndef display_h
#define display_h

#include <stdio.h>
#include <inttypes.h>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* Max Valid Coordinate: (63, 31) */
#define TERM_X  500
#define TERM_Y  500

#define NUM_SPRITES 16
#define BYTES_PER_SPRITE 5

#define clear() printf("\033[H\033[J")
void display_init();
void draw_screen();
void screenclear();
void raise_frame();
void lower_frame();
bool check_frame();

uint8_t* get_screen();

/* Screen display thread */
void* screen_display();

/* Screen mutex */
void get_screen_mutex();
void release_screen_mutex();
void reshape_window(GLsizei w, GLsizei h);

#endif /* display_h */
