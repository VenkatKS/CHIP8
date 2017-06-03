//
//  graphics_manager.h
//  CHIP8
//
//  Created by Venkat Srinivasan on 6/2/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifndef graphics_manager_h
#define graphics_manager_h

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include <stdio.h>
#include <stdbool.h>

/* Initialize */
void graphics_manager_init();

/* Get the current GAME screen data */
uint8_t *get_game_screen();

/* Set the current screen data */
typedef struct {
	uint64_t	game_width;
	uint64_t	game_height;
} GAME_SCREEN_DIMS;

/* Screen display thread */
void* screen_display();

/* Screen mutex */
void reshape_window(GLsizei w, GLsizei h);

/* Screen Control */
void game_screen_clear();
uint8_t *get_game_screen();
void set_game_screen(uint8_t *local_screen);
bool check_frame();

/* Game Control Handler */
void draw_screen();
GAME_SCREEN_DIMS *get_game_dimensions();
void graphics_manager_run();


#endif /* graphics_manager_h */
