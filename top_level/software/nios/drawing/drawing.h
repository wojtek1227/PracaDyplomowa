/*
 * drawing.h
 *
 *  Created on: 17 gru 2017
 *      Author: caput
 */

#ifndef DRAWING_H_
#define DRAWING_H_

#include <stdint.h>
#include <stdbool.h>
#include "system.h"


// X position can be adjusted to the nearest byte in the frame buffer <= 80 bytes per line
#define MAX_X 80
//Y position can be adjusted to the nearest pixel
#define MAX_Y 459


#define SHOT_SIZE_X 2
#define SHOT_SIZE_Y 16

#define PLAYER_SIZE_X 4
#define PLAYER_SIZE_Y 32

#define ALIEN1_SIZE_X 4
#define ALIEN1_SIZE_Y 32

#define LETTER_SIZE_X 2
#define LETTER_SIZE_Y 16

#define NUMBER_SIZE_X 1
#define NUMBER_SIZE_Y 16

typedef enum Object_type
{
	player_type,
	shot_type,
	alien_type,
}Object_type;

void draw_object(uint16_t x, uint16_t y, Object_type object_type);
//void draw_player(uint16_t x, uint16_t y);
//void draw_alien1(uint16_t x, uint16_t y);
//void draw_shot(uint16_t x, uint16_t y);
void erase_draw(uint16_t x, uint16_t y, Object_type object_type);
void clear_screen(void);


#endif /* DRAWING_H_ */
