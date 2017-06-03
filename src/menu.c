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
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
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
uint64_t all_roms_menu = 0;

extern pthread_t decoder_thread_tid;

uint64_t rom_id = ALL_ROMS_RSVD_START;

char *all_roms[100];

void populate_menu(char *dir, int depth)
{
	DIR *dp;
	char cur_cwd[1024];
	uint64_t idx = 0;

	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(dir)) == NULL) {
		fprintf(stderr,"Directory cannot be scanned: %s\n", dir);
		return;
	}

	chdir(dir);
	while((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)) {
			if(strcmp(".",entry->d_name) == 0 ||
			   strcmp("..",entry->d_name) == 0)
				continue;

			/* Recurse at a new indent level */
			populate_menu(entry->d_name,depth+4);
		}
		else {
			getcwd(cur_cwd, sizeof(cur_cwd));
			if (strstr(entry->d_name, ".c8") != NULL) {
				/* Store the path to this ROM so we can load it later */
				idx = rom_id - ALL_ROMS_RSVD_START;
				/* No need to worry about cleaning up this alloc, we need this till the end */
				all_roms[idx] = malloc(sizeof(cur_cwd));
				strcpy(all_roms[idx], cur_cwd);
				strcat(all_roms[idx], "/");
				strcat(all_roms[idx], entry->d_name);
				glutAddMenuEntry(entry->d_name, (int) rom_id++);
			}
		}
	}
	chdir("..");
	closedir(dp);
}

/* Handles the actions behind the various menu choices */
void menu_handler(int choice)
{
	char filelocation[1024];
	int idx = 0;

	/* Menu Action Handler */
	MENU_ACTION action_idx = (MENU_ACTION) choice;
	switch (action_idx) {
		case QUIT_APPLICATION:
			exit(0);
		case INCREASE_EMULATION_SPEED:
		case DECREASE_EMULATION_SPEED:
		case PAUSE_EMULATION:
			printf("Functions not yet implemented!");
			break;
		case LOAD_ROM:
			stop_decoder();
			printf("Please Enter ROM Path: ");
			scanf("%s", filelocation);
			set_pc(STARTING_PC);
			game_screen_clear();
			rom_init(filelocation);
			start_decoder();
			break;
		default:
			if (choice < ALL_ROMS_RSVD_START) mypanic("Unimplemented menu choice.");
			idx = choice - ALL_ROMS_RSVD_START;
			strcpy(filelocation, all_roms[idx]);
			stop_decoder();
			set_pc(STARTING_PC);
			game_screen_clear();
			rom_init(filelocation);
			start_decoder();
			break;
	}
}

void menu_init(int window_id)
{
	char cwd[1024];

	/* Determine this directory */
	getcwd(cwd, sizeof(cwd));

	sub_menu = glutCreateMenu(menu_handler);
	glutAddMenuEntry("Pause Emulation", PAUSE_EMULATION);
	glutAddMenuEntry("Increase Emulation Speed", INCREASE_EMULATION_SPEED);
	glutAddMenuEntry("Decrease Emulation Speed", DECREASE_EMULATION_SPEED);
	all_roms_menu = glutCreateMenu(menu_handler);
	printf("Scanning for ROM Files under: %s\n", cwd);
	populate_menu(cwd, 0);

	/* Warn the user */
	if (rom_id == ALL_ROMS_RSVD_START + 1) {
		printf("NO ROMS FOUND! Please place ROMs in the same folder as emulator. Please ensure ROMs have .c8 extension.\n");
	}

	main_menu = glutCreateMenu(menu_handler);
	glutAddSubMenu("Emulation Controls", (int) sub_menu);
	glutAddSubMenu("Load New ROM", (int) all_roms_menu);
	glutAddMenuEntry("Quit", QUIT_APPLICATION);
	glutAddMenuEntry("Open ROM From Path (terminal input)", LOAD_ROM);
	glutAttachMenu(GLUT_LEFT_BUTTON);

}

