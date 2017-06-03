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
#include "app_strings.h"

uint64_t main_menu = 0;
uint64_t sub_menu = 0;
uint64_t all_roms_menu = 0;
uint64_t roms_found = 0;
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
		fprintf(stderr, DIR_SCAN_ERROR_MSG);
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
				roms_found++;
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
			return;
		case INCREASE_EMULATION_SPEED:
		case DECREASE_EMULATION_SPEED:
		case PAUSE_EMULATION:
			printf("TODO: Functions not yet implemented!");
			break;
		case LOAD_ROM:
			stop_decoder();
			printf(ROM_PATH_PROMPT_MSG);
			scanf("%s", filelocation);
			set_pc(STARTING_PC);
			game_screen_clear();
			rom_init(filelocation);
			start_decoder();
			break;
		default:
			if (roms_found == 0) mypanic("Unimplemented menu choice.");
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
	glutAddMenuEntry(PAUSE_EMULATION_LABEL, PAUSE_EMULATION);
	glutAddMenuEntry(SPEED_UP_EMULATION_LABEL, INCREASE_EMULATION_SPEED);
	glutAddMenuEntry(SLOW_DOWN_EMULATION_LABEL, DECREASE_EMULATION_SPEED);
	all_roms_menu = glutCreateMenu(menu_handler);
	printf(SCANNING_FOR_ROMS_MSG);
	populate_menu(cwd, 0);

	/* Warn the user */
	if (roms_found == 0) {
		printf(NO_ROM_FOUND_MSG);
	}

	main_menu = glutCreateMenu(menu_handler);
	glutAddSubMenu(EMULATION_CONTROL_LABEL, (int) sub_menu);
	glutAddSubMenu(LOAD_NEW_ROM_LABEL, (int) all_roms_menu);
	glutAddMenuEntry(QUIT_LABEL, QUIT_APPLICATION);
	glutAddMenuEntry(OPEN_RAW_PATH_ROM_LABEL, LOAD_ROM);
	glutAttachMenu(GLUT_LEFT_BUTTON);

}

