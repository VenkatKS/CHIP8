//
//  menu.c
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
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include "menu.h"
#include "decoder.h"
#include "rommanager.h"
#include "services.h"
#include "memory.h"
#include "regs.h"
#include "graphics_manager.h"

uint64_t main_menu = 0;
uint64_t sub_menu = 0;

extern pthread_t decoder_thread_tid;

void menu_handler(int choice)
{
	char filelocation[100];
	/* Menu Action Handler */
	MENU_ACTION action_idx = (MENU_ACTION) choice;
	switch (action_idx) {
		case QUIT_APPLICATION:
			exit(0);
			return;
		case INCREASE_EMULATION_SPEED:
		case DECREASE_EMULATION_SPEED:
		case PAUSE_EMULATION:
			printf("Functions not yet implemented!");
		case LOAD_ROM:
			stop_decoder();
			printf("Please Enter ROM Path: ");
			scanf("%s", filelocation);
			set_pc(STARTING_PC);
			game_screen_clear();
			rom_init(filelocation);
			start_decoder();
			return;
		default:
			mypanic("Error! Invalid menu choice!");
			return;
	}
}

void menu_init(int window_id)
{
	sub_menu = glutCreateMenu(menu_handler);
	glutAddMenuEntry("Pause Emulation", PAUSE_EMULATION);
	glutAddMenuEntry("Increase Emulation Speed", INCREASE_EMULATION_SPEED);
	glutAddMenuEntry("Decrease Emulation Speed", DECREASE_EMULATION_SPEED);
	main_menu = glutCreateMenu(menu_handler);
	glutAddSubMenu("Emulation Controls", (int) sub_menu);
	glutAddMenuEntry("Load ROM", LOAD_ROM);
	glutAddMenuEntry("Quit", QUIT_APPLICATION);
	glutAttachMenu(GLUT_LEFT_BUTTON);

}

