//
//  main.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/21/17.
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
#include "regs.h"
#include "memory.h"
#include "decoder.h"
#include "display.h"
#include "graphics_manager.h"
#include "keyboard.h"
#include "services.h"
#include "rommanager.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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
	glutInit(&argc, argv);
	graphics_manager_init();

	// Setup OpenGL
	display_init();
	if (pthread_create(&decoder_thread_tid, NULL, &run_me, NULL) != 0) {
		mypanic("Cannot create decoder thread!\n");
		return 0;
	}
	graphics_manager_run();
}
