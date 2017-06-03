//
//  menu.h
//  CHIP8
//
//  Created by Venkat Srinivasan on 6/2/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifndef menu_h
#define menu_h

#include <stdio.h>

void menu_init(int window_id);
void menu_handler(int choice);

typedef enum {
	QUIT_APPLICATION,
	LOAD_ROM,
	INCREASE_EMULATION_SPEED,
	DECREASE_EMULATION_SPEED,
	PAUSE_EMULATION,
	ALL_ROMS_RSVD_START
	/* All enumerations below this is reserved for ROMs in the folder */
} MENU_ACTION;
#endif /* menu_h */
