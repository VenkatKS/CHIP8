//
//  display.h
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifndef display_h
#define display_h

#include <stdio.h>
#include <inttypes.h>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_SPRITES 16
#define BYTES_PER_SPRITE 5

void sprite_init();

#endif /* display_h */
