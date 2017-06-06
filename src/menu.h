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
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	ALL_ROMS_RSVD_START
	/* All enumerations below this is reserved for ROMs in the folder */
} MENU_ACTION;
#endif /* menu_h */
