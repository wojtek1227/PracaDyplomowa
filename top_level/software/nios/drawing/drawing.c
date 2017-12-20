/*
 * drawing.c
 *
 *  Created on: 17 gru 2017
 *      Author: caput
 */
#include "drawing.h"

static volatile uint8_t* const frame_buffer = (uint8_t*)VRAM_BASE;

static const uint8_t player_pbm[PLAYER_SIZE_Y * PLAYER_SIZE_X] =
{
	0b00000000,	0b00000011, 0b11000000, 0b00000000,
	0b00000000,	0b00000011, 0b11000000, 0b00000000,
	0b00000000,	0b00000111, 0b11100000, 0b00000000,
	0b00000000,	0b00000111, 0b11100000, 0b00000000,
	0b00000000,	0b00000111, 0b11100000, 0b00000000,
	0b00000000,	0b00000111, 0b11100000, 0b00000000,
	0b00000000,	0b00001111, 0b11110000, 0b00000000,
	0b00000000,	0b00001111, 0b11110000, 0b00000000,
	0b00000000,	0b01001111, 0b11110010, 0b00000000,
	0b00000000,	0b01011111, 0b11111010, 0b00000000,
	0b00000000,	0b01011111, 0b11111010, 0b00000000,
	0b00000000,	0b01011110, 0b01111010, 0b00000000,
	0b00000000,	0b01011110, 0b01111010, 0b00000000,
	0b00000000,	0b01111110, 0b01111110, 0b00000000,
	0b00000100,	0b01111100, 0b00111110, 0b00100000,
	0b00000100,	0b01111101, 0b10111110, 0b00100000,
	0b00000110,	0b01111001, 0b10011110, 0b01100000,
	0b00001110,	0b01111011, 0b11011110, 0b01110000,
	0b00001110,	0b01111111, 0b11111110, 0b01110000,
	0b00001110,	0b11111111, 0b11111111, 0b01110000,
	0b00001110,	0b11111111, 0b11111111, 0b01110000,
	0b00001110,	0b11111111, 0b11111111, 0b01110000,
	0b00001110,	0b11111111, 0b11111111, 0b01110000,
	0b00001111,	0b11111111, 0b11111111, 0b11110000,
	0b00001111,	0b11111111, 0b11111111, 0b11110000,
	0b00001111,	0b11111111, 0b11111111, 0b11110000,
	0b00001111,	0b11111101, 0b10111111, 0b11110000,
	0b00001111,	0b11111001, 0b10011111, 0b11110000,
	0b00011111,	0b11110001, 0b10001111, 0b11111000,
	0b00111111,	0b11100001, 0b10000111, 0b11111100,
	0b01111111,	0b11000001, 0b10000011, 0b11111110,
	0b11111111,	0b10000001, 0b10000001, 0b11111111
};

static const uint8_t shot_pbm[SHOT_SIZE_Y * SHOT_SIZE_X] =
{
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000011, 0b11000000,
	0b00000111, 0b11100000,
	0b00001111, 0b11110000,
	0b00001111, 0b11110000,
	0b00001111, 0b11110000,
	0b00001111, 0b11110000,
	0b00000111, 0b11100000,
	0b00000011, 0b11000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000
};

static const uint8_t alien1_pbm[ALIEN1_SIZE_Y * ALIEN1_SIZE_X] =
{
	0b00000000, 0b00001111, 0b11110000, 0b00000000,
	0b00000000, 0b01111111, 0b11111110, 0b00000000,
	0b00000000, 0b11111111, 0b11111111, 0b00000000,
	0b00000011, 0b11111111, 0b11111111, 0b11000000,
	0b00000111, 0b11111111, 0b11111111, 0b11100000,
	0b00001111, 0b11111111, 0b11111111, 0b11110000,
	0b00011111, 0b11111111, 0b11111111, 0b11111100,
	0b00111111, 0b11111100, 0b00111111, 0b11111110,
	0b11111111, 0b11111000, 0b00011111, 0b11111111,
	0b11111111, 0b11110000, 0b00001111, 0b11111111,
	0b11111111, 0b11110001, 0b10001111, 0b11111111,
	0b11111111, 0b11110000, 0b00001111, 0b11111111,
	0b11111111, 0b11111000, 0b00011111, 0b11111111,
	0b11111111, 0b11111100, 0b00111111, 0b11111111,
	0b00111111, 0b11111111, 0b11111111, 0b11111100,
	0b00011111, 0b11111111, 0b11111111, 0b11111000,
	0b00001111, 0b11111111, 0b11111111, 0b11110000,
	0b00000111, 0b11111111, 0b11111111, 0b11100000,
	0b00000011, 0b11111111, 0b11111111, 0b11000000,
	0b00000111, 0b11111111, 0b11111111, 0b11100000,
	0b00000111, 0b01111111, 0b11111110, 0b11100000,
	0b00001110, 0b00001111, 0b11110000, 0b01110000,
	0b00001110, 0b00000000, 0b00000000, 0b01110000,
	0b00011100, 0b00000000, 0b00000000, 0b00111000,
	0b00011100, 0b00000000, 0b00000000, 0b00111000,
	0b00111000, 0b00000000, 0b00000000, 0b00011100,
	0b00111000, 0b00000000, 0b00000000, 0b00011100,
	0b00111000, 0b00000000, 0b00000000, 0b00011100,
	0b00111000, 0b00011110, 0b01111000, 0b00011100,
	0b00011100, 0b00110000, 0b00001100, 0b00111000,
	0b00011110, 0b01000000, 0b00000010, 0b01111000,
	0b00001111, 0b10000000, 0b00000001, 0b11110000
};

static const uint8_t numbers[10][16] =
{
	{
		0b00111100, 0b01111110, 0b11000011, 0b11000011, 0b11000011,
		0b11000011, 0b11000011, 0b11000011, 0b11000011, 0b11000011,
		0b11000011, 0b11000011, 0b11000011, 0b11000011, 0b01111110,
		0b00111100
	},
	{
		0b11000000, 0b11100000, 0b11110000, 0b11111000, 0b11111100,
		0b11111110, 0b11101111, 0b11100111, 0b11100011, 0b11100000,
		0b11100000, 0b11100000, 0b11100000, 0b11100000, 0b11100000,
		0b11100000
	},
	{
		0b00111100, 0b01111110, 0b11111111, 0b11100011, 0b11000001,
		0b11000001, 0b11100000, 0b11110000, 0b01111000, 0b00111100,
		0b00011110, 0b00001111, 0b00000111, 0b11111111, 0b11111111,
		0b11111111
	},
	{
		0b01111000, 0b01111100, 0b11101110, 0b11000111, 0b11000011,
		0b11100000, 0b11100000, 0b11111000, 0b11111000, 0b11100000,
		0b11000000, 0b11000001, 0b11000011, 0b11100111, 0b01111110,
		0b00111100
	},
	{
		0b11000000, 0b11100000, 0b11110000, 0b11111000, 0b11011100,
		0b11001100, 0b11000110, 0b11000111, 0b11111111, 0b11111111,
		0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000,
		0b11000000
	},
	{
		0b11111111, 0b11111111, 0b00000011, 0b00000011, 0b00000011,
		0b00000011, 0b01111111, 0b01111111, 0b11100000, 0b11000000,
		0b11000000, 0b11000011, 0b11100011, 0b11111111, 0b01111110,
		0b01111110
	},
	{
		0b00111100, 0b01111110, 0b11000011, 0b00000001, 0b00000001,
		0b00000001, 0b00000001, 0b00111101, 0b01111111, 0b11000011,
		0b10000001, 0b10000001, 0b10000001, 0b11000011, 0b01111110,
		0b00111100
	},
	{
		0b11111111, 0b11111111, 0b11000000, 0b11000000, 0b11100000,
		0b01100000, 0b01110000, 0b00110000, 0b00111000, 0b00011000,
		0b00011100, 0b00001100, 0b00001110, 0b00000110, 0b00000111,
		0b00000011
	},
	{
		0b00000000, 0b00111100, 0b01111110, 0b11000011, 0b11000011,
		0b11000011, 0b11000011, 0b01111110, 0b00111100, 0b01111110,
		0b11000011, 0b11000011, 0b11000011, 0b11000011, 0b01111110,
		0b00111100
	},
	{
		0b00111100, 0b01111110, 0b11000011, 0b10000001, 0b10000001,
		0b10000001, 0b11000011, 0b11111110, 0b10111100, 0b10000000,
		0b10000000, 0b10000000, 0b10000000, 0b11000011, 0b01111110,
		0b00111100
	}

};

static const uint8_t T[LETTER_SIZE_X * LETTER_SIZE_Y] =
{
	0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b00000011, 0b11000000, 0b00000011, 0b11000000,
	0b00000011, 0b11000000, 0b00000011, 0b11000000, 0b00000011,
	0b11000000, 0b00000011, 0b11000000, 0b00000011, 0b11000000,
	0b00000011, 0b11000000, 0b00000011, 0b11000000, 0b00000011,
	0b11000000, 0b00000011, 0b11000000, 0b00000011, 0b11000000,
	0b00000011, 0b11000000
};

static const uint8_t i[LETTER_SIZE_X * LETTER_SIZE_Y] =
{
	0b00000000, 0b00000000, 0b00000011, 0b11000000, 0b00000011,
	0b11000000, 0b00000011, 0b11000000, 0b00000011, 0b11000000,
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
	0b00000000, 0b00000011, 0b11000000, 0b00000011, 0b11000000,
	0b00000011, 0b11000000, 0b00000011, 0b11000000, 0b00000011,
	0b11000000, 0b00000011, 0b11000000, 0b00000011, 0b11000000,
	0b00000011, 0b11000000
};

static const uint8_t m[LETTER_SIZE_X * LETTER_SIZE_Y] =
{
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111,
	0b00111111, 0b11111111, 0b01111111, 0b11111111, 0b11111111,
	0b11111111, 0b11100011, 0b10000111, 0b11100011, 0b10000111,
	0b11100011, 0b10000111, 0b11100011, 0b10000111, 0b11100011,
	0b10000111, 0b11100011, 0b10000111, 0b11100011, 0b10000111,
	0b11100011, 0b10000111
};

static const uint8_t e[LETTER_SIZE_X * LETTER_SIZE_Y] =
{
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
	0b00000000, 0b00000111, 0b11110000, 0b00001111, 0b11111000,
	0b00011000, 0b00001100, 0b00010000, 0b00000100, 0b00110000,
	0b00000110, 0b00111111, 0b11111110, 0b00111111, 0b11111110,
	0b00111111, 0b11111110, 0b00000000, 0b00000110, 0b00000000,
	0b00000110, 0b00000000, 0b00001100, 0b00001111, 0b11111000,
	0b00000111, 0b11110000
};

static const uint8_t comma[NUMBER_SIZE_X * NUMBER_SIZE_Y] =
{
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00000000, 0b00011000, 0b00011000,
	0b00011000
};

void draw_object(uint16_t x, uint16_t y, Object_type object_type)
{
	uint8_t object_x_size, object_y_size;
	const uint8_t * pattern;
	switch (object_type) {
		case player_type:
			object_x_size = PLAYER_SIZE_X;
			object_y_size = PLAYER_SIZE_Y;
			pattern = player_pbm;
			break;
		case shot_type:
			object_x_size = SHOT_SIZE_X;
			object_y_size = SHOT_SIZE_Y;
			pattern = shot_pbm;
			break;
		case alien_type:
			object_x_size = ALIEN1_SIZE_X;
			object_y_size = ALIEN1_SIZE_Y;
			pattern = alien1_pbm;
			break;
		default:
			break;
	}
	if ((x > 0) && (x < MAX_X - object_x_size) && (y > 0) && (y < MAX_Y - object_y_size))
	{
		for(uint8_t i = 0; i < object_y_size; i++)
		{
			for(uint8_t j = 0; j < object_x_size; j++)
			{
				*((uint8_t*)VRAM_BASE + (y + i) * MAX_X + x + j) |= pattern[i * object_x_size + object_x_size - 1 - j];
			}
		}
	}
}

void erase_draw(uint16_t x, uint16_t y, Object_type object_type)
{
	uint8_t object_x_size, object_y_size;
	const uint8_t * pattern;
	switch (object_type) {
		case player_type:
			object_x_size = PLAYER_SIZE_X;
			object_y_size = PLAYER_SIZE_Y;
			pattern = player_pbm;
			break;
		case shot_type:
			object_x_size = SHOT_SIZE_X;
			object_y_size = SHOT_SIZE_Y;
			pattern = shot_pbm;
			break;
		case alien_type:
			object_x_size = ALIEN1_SIZE_X;
			object_y_size = ALIEN1_SIZE_Y;
			pattern = alien1_pbm;
			break;
		default:
			break;
	}
	if ((x > 0) && (x < MAX_X - object_x_size) && (y > 0) && (y < MAX_Y - object_y_size))
	{
		for(uint8_t i = 0; i < object_y_size; i++)
		{
			for(uint8_t j = 0; j < object_x_size; j++)
			{
				*((uint8_t*)VRAM_BASE + (y + i) * MAX_X + x + j) &= ~pattern[i * object_x_size + object_x_size - 1 - j];
			}
		}
	}
}

//void draw_player(uint16_t x, uint16_t y)
//{
//	if ((x > 0) && (x < MAX_X - PLAYER_SIZE_X) && (y > 0) && (y < MAX_Y - PLAYER_SIZE_Y))
//	{
//		for(uint8_t i = 0; i < PLAYER_SIZE_Y; i++)
//		{
//			for(uint8_t j = 0; j < PLAYER_SIZE_X; j++)
//			{
//				*((uint8_t*)VRAM_BASE + (y + i) * MAX_X + x + j) = player_pbm[i * PLAYER_SIZE_X + 3 - j];
//			}
//		}
//	}
//}
//
//void draw_alien1(uint16_t x, uint16_t y)
//{
//	if ((x > 0) && (x < MAX_X - ALIEN1_SIZE_X) && (y > 0) && (y < MAX_Y - ALIEN1_SIZE_Y))
//	{
//		for(uint8_t i = 0; i < ALIEN1_SIZE_Y; i++)
//		{
//			for(uint8_t j = 0; j < ALIEN1_SIZE_X; j++)
//			{
//				*((uint8_t*)VRAM_BASE + (y + i) * MAX_X + x + j) = alien1_pbm[i * ALIEN1_SIZE_X + 3 - j];
//			}
//		}
//	}
//}
//
//void draw_shot(uint16_t x, uint16_t y)
//{
//	if ((x > 0) && (x < MAX_X - SHOT_SIZE_X) && (y > 0) && (y < MAX_Y - SHOT_SIZE_Y))
//	{
//		for(uint8_t i = 0; i < SHOT_SIZE_Y; i++)
//		{
//			for(uint8_t j = 0; j < SHOT_SIZE_X; j++)
//			{
//				*((uint8_t*)VRAM_BASE + (y + i) * MAX_X + x + j) = shot_pbm[i * SHOT_SIZE_X + 1 - j];
//			}
//		}
//	}
//}


void clear_screen(void)
{
	for(uint32_t i = 0; i < VRAM_SIZE_VALUE; i++)
	{
		*((uint8_t*)VRAM_BASE + i) = 0x00;
	}
}
