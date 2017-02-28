//
//  keyboard.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/23/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#include "keyboard.h"
#include "services.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>

bool key_state[NUM_KEYS];
bool keyb_initialized = false;

pthread_t keyb_tid;

#define CALL_RETRY(retvar, expression) do { \
retvar = (expression); \
} while (retvar == -1 && errno == EINTR);

void keyboard_init()
{
    memset(key_state, 0, NUM_KEYS);

    /* Disable need to press enter before sending terminal char to stdin */
    system ("/bin/stty raw");
    /* Disable echo */
    system ("stty -echo");

    keyb_initialized = true;
}

int16_t get_keyMap(int keyCharacter)
{
    switch (keyCharacter) {
        case '1':
            return (1);
        case '2':
            return (2);
        case '3':
            return (3);
        case '4':
            return (12);
        case 'q':
        case 'Q':
            return (4);
        case 'w':
        case 'W':
            return (5);
        case 'e':
        case 'E':
            return (6);
        case 'r':
        case 'R':
            return (13);
        case 'a':
        case 'A':
            return (7);
        case 's':
        case 'S':
            return (8);
        case 'd':
        case 'D':
            return (9);
        case 'f':
        case 'F':
            return (14);
        case 'z':
        case 'Z':
            return (10);
        case 'x':
        case 'X':
            return (0);
        case 'c':
        case 'C':
            return (11);
        case 'v':
        case 'V':
            return (15);
        default:
            /* Not an actual emulated key */
            return (-1);
            break;
    }
}

bool check_keyPressed(uint8_t keyId)
{
    if (keyId > 0xF) mypanic("Illegal keyId\n");
    
    int16_t keyPressedId = -1;
    int val_avail = 0;
    fd_set set;
    struct timeval timeout;
    
    FD_ZERO (&set);
    FD_SET (STDIN_FILENO, &set);
    
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    
    CALL_RETRY (val_avail, select (FD_SETSIZE, &set, NULL, NULL, &timeout));
    if (val_avail == 0) return false;
    
    int keyp = fgetc(stdin);
    keyPressedId = get_keyMap(keyp);
    
    if (keyPressedId == -1) return false;
    if (keyPressedId == keyId) return true;
    else return false;
}

int16_t get_keyPressed()
{
    int val_avail = 0;
    fd_set set;
    struct timeval timeout;
    
    FD_ZERO (&set);
    FD_SET (STDIN_FILENO, &set);
    
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    
    CALL_RETRY (val_avail, select (FD_SETSIZE, &set, NULL, NULL, &timeout));
    if (val_avail == 0) return -1;

    int keyp = fgetc(stdin);

    return get_keyMap(keyp);
}
