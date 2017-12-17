/*
 * game.c
 *
 *  Created on: 17 gru 2017
 *      Author: caput
 */
#include "game.h"

typedef enum Object_type
{
	player,
	player_shot,
	alien,
	alien1_shot
}Object_type;

typedef struct Object
{
	uint16_t x_coordinate;
	uint16_t y_coordinate;
	uint8_t x_size;
	uint8_t y_size;
	Object_type type;
	uint8_t is_alive; //if object is not alive it won't be drwawn
	int8_t relative_pos; //position relative to start
	int8_t velocity;
}Object;

//Static variables
static const uint8_t number_of_aliens_rows = 3;
static const uint8_t number_of_aliens_cols = 6;
static const uint8_t number_of_aliens = 18;
static const uint8_t aliens_init_pos_x = 22;
static const uint8_t aliens_init_pos_y = 20;
static const uint8_t space_between_aliens_x = 2;
static const uint8_t space_between_aliens_y = 16;
static const uint8_t aliens_velocity = 1;

static const uint8_t number_of_player_shots = 5;

//Static functions
static void init_aliens(Object *aliens)
{
	for(uint8_t i = 0; i < number_of_aliens_rows; i++)
	{
		for(uint8_t j = 0; j < number_of_aliens_cols; j++)
		{
			(aliens + i * number_of_aliens_cols + j)->type = alien;
			(aliens + i * number_of_aliens_cols + j)->is_alive = 0x01;
			(aliens + i * number_of_aliens_cols + j)->x_size = ALIEN1_SIZE_X;
			(aliens + i * number_of_aliens_cols + j)->y_size = ALIEN1_SIZE_Y;
			(aliens + i * number_of_aliens_cols + j)->x_coordinate = aliens_init_pos_x + j *(ALIEN1_SIZE_X + space_between_aliens_x);
			(aliens + i * number_of_aliens_cols + j)->y_coordinate = aliens_init_pos_y + i * (ALIEN1_SIZE_Y + space_between_aliens_y);
			(aliens + i * number_of_aliens_cols + j)->relative_pos = 0;
			(aliens + i * number_of_aliens_cols + j)->velocity = aliens_velocity;
			draw_alien1((aliens + i * number_of_aliens_cols + j)->x_coordinate, (aliens + i * number_of_aliens_cols + j)->y_coordinate);
		}
	}
}
//
static void init_shots(Object *shots)
{
	for(uint8_t i = 0;i < number_of_player_shots; i++)
	{
		(shots + i)->type = player_shot;
		(shots + i)->is_alive = 0x00;
		(shots + i)->x_size = SHOT_SIZE_X;
		(shots + i)->y_size = SHOT_SIZE_Y;
	}
}

static void move(Object *objects, uint8_t number_of_objects)
{
	for(uint8_t i = 0; i < number_of_objects; i++)
	{
		switch ((objects + i)->type)
		{
			case alien:
				if((objects + i)->is_alive)
				{

					clear_object((objects + i)->x_coordinate,(objects + i)->y_coordinate, (objects + i)->x_size, (objects + i)->y_size);
					if(((objects + i)->relative_pos == -10) || ((objects + i)->relative_pos == 10))
					{
						(objects + i)->velocity *= -1;
					}
					(objects + i)->x_coordinate += (objects + i)->velocity;
					(objects + i)->relative_pos += (objects + i)->velocity;
					draw_alien1((objects + i)->x_coordinate, (objects + i)->y_coordinate);
				}

				break;
			default:
				break;
		}
	}
}
void game_run(void)
{
	uint32_t time;
	Object aliens[number_of_aliens];
	Object player_shots[number_of_player_shots];
	init_aliens(aliens);

	time = get_time();
	while(1)
	{
		if((get_time() - time) == 500)
		{
			move(aliens, number_of_aliens);
			time = get_time();
		}
	}

}

