//
//  display.c
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#include "display.h"
#include "services.h"
#include "memory.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t display_mutex;

uint8_t chip8_fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

uint8_t screen[TERM_X * TERM_Y];

uint8_t* get_screen()
{
    return screen;
}

void get_screen_mutex()
{
    pthread_mutex_lock(&display_mutex);
}

void release_screen_mutex()
{
    pthread_mutex_unlock(&display_mutex);
}

void screenclear()
{
    clear();
    fflush(0);
    memset(screen, 0, TERM_X * TERM_Y);
}

void draw_screen()
{
    pthread_mutex_lock(&display_mutex);
    clear();
    fflush(0);
    uint32_t i, j = 0;
    for (i = 0; i < TERM_X; i++) {
        for (j = 0; j < TERM_Y; j++) {
            if (screen[i + ((j) * TERM_X)]) printf("\033[%d;%dH*", j, i);
            /* Immediately flush stdio */
            fflush(0);
        }
    }
    pthread_mutex_unlock(&display_mutex);
}

void display_init()
{
    uint16_t xIterator, yIterator, memoryIterator = 0;
    memset(screen, 0, TERM_X * TERM_Y);
    draw_screen();
    ACCESSPRINT(printf("Loading digit sprites into memory.\n"););
    for (xIterator = 0; xIterator < NUM_SPRITES; xIterator++) {
        for (yIterator = 0; yIterator < BYTES_PER_SPRITE; yIterator++) {
            access_memory(memoryIterator++, MEM_ST8, &chip8_fontset[(xIterator * BYTES_PER_SPRITE) + yIterator]);
        }
    }
    ACCESSPRINT(printf("Finished loading digit sprites into memory.\n"););
    pthread_mutex_init(&display_mutex, NULL);
}

/* Display thread */
void* screen_display()
{
    struct timespec time_duration;
    time_duration.tv_sec = 0;
    time_duration.tv_nsec = 10000000;
    
    while (true) {
        draw_screen();
        nanosleep(&time_duration, NULL);
    }
}
