//
//  graphics_manager.c
//  CHIP8
//
//  Created by Venkat Srinivasan on 6/2/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#include <pthread.h>
#include <string.h>
#include "graphics_manager.h"
#include "sprite_manager.h"
#include "keyboard.h"
#include "menu.h"

/* Window Properties */
uint64_t window_id;

/* The Entire Screen */
#define SCREEN_WIDTH		500
#define SCREEN_HEIGHT		500
#define PIXEL_MAGNIFICATION	10

#define RESERVED_MARGIN_WIDTH	100
#define RESERVED_MARGIN_HEIGHT	100

#define GAME_WIDTH		400
#define GAME_HEIGHT		400

uint8_t screen[SCREEN_WIDTH * SCREEN_HEIGHT];
uint8_t top_bar[RESERVED_MARGIN_HEIGHT * SCREEN_WIDTH];
uint8_t game_screen[GAME_WIDTH * GAME_HEIGHT];

static bool new_frame = false;

GAME_SCREEN_DIMS running_game_dimensions = {GAME_WIDTH, GAME_HEIGHT};

pthread_mutex_t display_mutex;

/* Call this for the first init */
void graphics_manager_init()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(SCREEN_WIDTH + 200, SCREEN_HEIGHT + 100);
	window_id = glutCreateWindow("Venkat CHIP8");
	menu_init((int) window_id);
	glutDisplayFunc(draw_screen);
	glutIdleFunc(draw_screen);
	glutReshapeFunc(reshape_window);
	glutKeyboardFunc(gl_keyDownHandler);
	glutKeyboardUpFunc(gl_keyUpHandler);
	pthread_mutex_init(&display_mutex, NULL);
}

/* Call this whenever the view changes */
void graphics_manager_reinit()
{
	glutDisplayFunc(draw_screen);
	glutIdleFunc(draw_screen);
	glutReshapeFunc(reshape_window);
	glutKeyboardFunc(gl_keyDownHandler);
	glutKeyboardUpFunc(gl_keyUpHandler);
}

void graphics_manager_run()
{
	glutMainLoop();
}

static void get_screen_mutex()
{
	pthread_mutex_lock(&display_mutex);
}

static void release_screen_mutex()
{
	pthread_mutex_unlock(&display_mutex);
}

static void raise_frame()
{
	new_frame = true;
}

static void lower_frame()
{
	new_frame = false;
}

bool check_frame()
{
	return new_frame;
}

uint8_t *get_game_screen()
{
	uint8_t *local_screen;
	get_screen_mutex();
	/* Need to be fast -- no reason to call debugging allocate func */
	local_screen = (uint8_t *) malloc(running_game_dimensions.game_width * running_game_dimensions.game_height);
	memcpy(local_screen, game_screen, running_game_dimensions.game_width * running_game_dimensions.game_height);
	release_screen_mutex();
	return local_screen;
}

void set_game_screen(uint8_t *local_screen)
{
	get_screen_mutex();
	memcpy(game_screen, local_screen, running_game_dimensions.game_width * running_game_dimensions.game_height);
	raise_frame();
	release_screen_mutex();
}

void game_screen_clear()
{
	get_screen_mutex();
	memset(game_screen, 0, GAME_WIDTH * GAME_HEIGHT);
	raise_frame();
	release_screen_mutex();
}

GAME_SCREEN_DIMS *get_game_dimensions()
{
	return &running_game_dimensions;
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

uint64_t get_windowid()
{
	return window_id;
}

#define SET_PIXEL_AT(_i, _j, _mag)							\
	glBegin(GL_QUADS);								\
	glVertex3f((_i * _mag) + 0.0f,     (_j * _mag) + 0.0f,	 0.0f);			\
	glVertex3f((_i * _mag) + 0.0f,     (_j * _mag) + _mag, 0.0f);			\
	glVertex3f((_i * _mag) + _mag, (_j * _mag) + _mag, 0.0f);			\
	glVertex3f((_i * _mag) + _mag, (_j * _mag) + 0.0f,	 0.0f);			\
	glEnd();

void draw_screen()
{
	if (!new_frame) return;
	glutSetWindow((int) window_id);
	glClear(GL_COLOR_BUFFER_BIT);
	uint8_t *local_screen = get_game_screen();
	uint32_t i, j = 0;
	for (i = 0; i < GAME_WIDTH; i++) {
		for (j = 0; j < GAME_HEIGHT; j++) {
			if (local_screen[i + ((j) * GAME_WIDTH)])
				glColor3f(1.0f, 1.0f, 1.0f);
			else
				glColor3f(0.0f, 0.0f, 0.0f);
			SET_PIXEL_AT((i), (j), PIXEL_MAGNIFICATION);
		}
	}
	free(local_screen);
	glutSwapBuffers();
	glFlush();
	lower_frame();
}
