//
//  app_strings.h
//  CHIP8
//
//  Created by Venkat Srinivasan on 6/3/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifndef app_strings_h
#define app_strings_h

/* Message Strings */
#define	NO_ROM_FOUND_MSG		\
	"NO ROMS FOUND! Please place ROMs in the same folder as emulator. Please ensure ROMs have .c8 extension.\n"

#define	SCANNING_FOR_ROMS_MSG		\
	"Scanning for ROM Files under: %s\n", cwd

#define	ROM_PATH_PROMPT_MSG		\
	"Please Enter ROM Path: "

/* Menu Item Labels */
#define	EMULATION_CONTROL_LABEL		"Emulation Controls"
#define	LOAD_NEW_ROM_LABEL		"Load New ROM"
#define	QUIT_LABEL			"Quit"
#define	OPEN_RAW_PATH_ROM_LABEL		"Open ROM From Path (from stdin)"
#define	PAUSE_EMULATION_LABEL		"Pause Emulation"
#define	SPEED_UP_EMULATION_LABEL	"Increase Emulation Speed"
#define	SLOW_DOWN_EMULATION_LABEL	"Decrease Emulation Speed"

/* Error Message Strings */
#define DIR_SCAN_ERROR_MSG		"Directory cannot be scanned: %s\n", dir



#endif /* app_strings_h */