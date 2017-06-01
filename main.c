//
//  main.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/21/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//
#include "regs.h"
#include "memory.h"
#include "decoder.h"
#include "display.h"
#include "keyboard.h"
#include "services.h"
#include "rommanager.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>



pthread_t decoder_thread_tid;
int main(int argc, char** argv) {
	if (argc < 2) mypanic("Please specify ROM file!\n");
	init_memory();
	keyboard_init();
	regs_init();
	rom_init(argv[1]);

	// Setup OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(TERM_X + 200, TERM_Y + 100);

	display_init();
	glutCreateWindow("Venkat CHIP8");

	glutDisplayFunc(draw_screen);
	glutIdleFunc(draw_screen);
	glutReshapeFunc(reshape_window);
	glutKeyboardFunc(gl_keyDownHandler);
	glutKeyboardUpFunc(gl_keyUpHandler);
	if (pthread_create(&decoder_thread_tid, NULL, &run_me, NULL) != 0) {
		mypanic("Cannot create decoder thread!\n");
		return 0;
	}
	glutMainLoop();
}
