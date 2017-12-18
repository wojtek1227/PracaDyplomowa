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

//typedef enum Color_t
//{
//	black = 0xff,
//	white = 0x00
//}Color_t;

//typedef enum Object_t
//{
//	player,
//	alien1,
//	shot
//}Object_t;

void draw_player(uint16_t x, uint16_t y);
void draw_alien1(uint16_t x, uint16_t y);
void draw_shot(uint16_t x, uint16_t y);
void erese_object(uint16_t x, uint16_t y, uint8_t x_size, uint8_t y_size);
void clear_screen(void);


#endif /* DRAWING_H_ */
