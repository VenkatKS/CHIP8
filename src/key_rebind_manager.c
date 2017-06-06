//
//  key_rebind_manager.c
//  CHIP8
//
//  Created by Venkat Srinivasan on 6/6/17.
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
#include "key_rebind_manager.h"
#include "graphics_manager.h"
#include "app_strings.h"
#include "keyboard.h"

int warning_window_id;


/* Warning Window Dimensions */
#define WARNING_WINDOW_HEIGHT	100
#define WARNING_HOZ_OFFSET	30
#define WARNING_WINDOW_WIDTH	600

/* Current Key To Rebind */
uint8_t	keyToRebind;

/* Current Window Status */
bool window_enabled = false;

void warning_key_down_handler(unsigned char keyid, int one, int two)
{
	/* TODO: Fix bug where window closed by user still pauses emulator */
	resetSpecificKeyBinding(keyToRebind, keyid);
	glutDestroyWindow(warning_window_id);
	glutSetWindow((int) get_windowid());
	graphics_manager_reinit();
	window_enabled = false;
}

void reshape_warning_window(GLsizei w, GLsizei h)
{
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}


void init_warning_screen(uint8_t key)
{
	/* Only Supports One Warning View At A Time */
	if (window_enabled) return;

	int oldWindowId = glutGetWindow();
	int screen_pos_x = glutGet((GLenum)GLUT_WINDOW_X);
	int screen_pos_y = glutGet((GLenum)GLUT_WINDOW_Y);
	int screen_size_x = glutGet((GLenum) GLUT_WINDOW_WIDTH);
	int screen_size_y = glutGet((GLenum) GLUT_WINDOW_HEIGHT);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WARNING_WINDOW_WIDTH, WARNING_WINDOW_HEIGHT);

	/* Set Position To Half Of Old Window */
	glutInitWindowPosition(screen_pos_x + (screen_size_x/2), screen_pos_y + (screen_size_y/2));

	warning_window_id = glutCreateWindow(WARNING_WINDOW_HEADING_LABEL);
	glutSetWindow(warning_window_id);
	glutDisplayFunc(draw_warning_screen);
	glutIdleFunc(draw_warning_screen);
	glutReshapeFunc(reshape_warning_window);
	glutKeyboardFunc(warning_key_down_handler);
	glutSetWindow(oldWindowId);
	keyToRebind = key;
	window_enabled = true;
}

void *currentfont;
void setFont(void *font)
{
	currentfont=font;
}

void RenderString(GLdouble x, GLdouble y, char* string)
{
	glColor3d(0.0, 0.0, 0.0);
	glRasterPos2d(x, y);
	for (int n=0; string[n]; n++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[n]);
	}
}

#define MAX_REBIND_STATEMENT_LENGTH	100
void draw_warning_screen()
{
	if (!window_enabled) return;

	char rebStatement[MAX_REBIND_STATEMENT_LENGTH];
	glutSetWindow(warning_window_id);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glutPostRedisplay();
	sprintf(rebStatement, KEY_REBIND_PROMPT_STRING);
	RenderString(WARNING_HOZ_OFFSET, WARNING_WINDOW_HEIGHT/2, rebStatement);
	glutSwapBuffers();
	glFlush();
}
