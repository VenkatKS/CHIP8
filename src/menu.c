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
#include "key_rebind_manager.h"
#include "app_strings.h"
#include "keyboard.h"

/* Menu IDs */
uint64_t main_menu = 0;
uint64_t sub_menu = 0;
uint64_t all_roms_menu = 0;
uint64_t keys_menu = 0;

uint64_t roms_found = 0;
extern pthread_t decoder_thread_tid;

uint64_t rom_id = ALL_ROMS_RSVD_START;

char *all_roms[100];

/* Iterates through each sub-directory and populates menu */
void populate_roms_menu(char *dir, int depth)
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
			populate_roms_menu(entry->d_name,depth+4);
		}
		else {
			getcwd(cur_cwd, sizeof(cur_cwd));
			if (strstr(entry->d_name, CHIP8_ROM_FILE_EXTENSION) != NULL) {
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

#define MAX_CHIP8_KEY_LEN	100
void populate_keys_submenu()
{
	int idx = 0;
	char statement[MAX_CHIP8_KEY_LEN];
	int enum_idx = (int) KEY_0;

	for (idx = 0; idx < NUM_KEYS; idx++) {
		sprintf(statement, "Re-bind: %x", idx);
		glutAddMenuEntry(statement, enum_idx);
		enum_idx++;
	}
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
			printf("TODO: Functions not yet implemented!");
			break;
		case PAUSE_EMULATION:
			if (get_decoder_state()) stop_decoder();
			else start_decoder();
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
		case KEY_0:
		case KEY_1:
		case KEY_2:
		case KEY_3:
		case KEY_4:
		case KEY_5:
		case KEY_6:
		case KEY_7:
		case KEY_8:
		case KEY_9:
		case KEY_A:
		case KEY_B:
		case KEY_C:
		case KEY_D:
		case KEY_E:
		case KEY_F:
			idx = (action_idx - KEY_0);
			init_warning_screen(idx);
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
	all_roms_menu = glutCreateMenu(menu_handler);
	printf(SCANNING_FOR_ROMS_MSG);
	populate_roms_menu(cwd, 0);

	/* Warn the user */
	if (roms_found == 0) {
		printf(NO_ROM_FOUND_MSG);
	}

	keys_menu = glutCreateMenu(menu_handler);
	populate_keys_submenu();

	main_menu = glutCreateMenu(menu_handler);
	glutAddSubMenu(LOAD_NEW_ROM_LABEL, (int) all_roms_menu);
	glutAddSubMenu(REBIND_KEYS_LABEL, (int) keys_menu);
	glutAddMenuEntry(OPEN_RAW_PATH_ROM_LABEL, LOAD_ROM);
	glutAddSubMenu(EMULATION_CONTROL_LABEL, (int) sub_menu);
	glutAddMenuEntry(QUIT_LABEL, QUIT_APPLICATION);
	glutAttachMenu(GLUT_LEFT_BUTTON);

}

