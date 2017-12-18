/*
 * game.c
 *
 *  Created on: 17 gru 2017
 *      Author: caput
 */
#include "game.h"

typedef enum Object_type
{
	player_type,
	player_shot_type,
	alien_type,
	alien1_shot_type
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
static const uint8_t aliens_init_pos_x = 23;
static const uint8_t aliens_init_pos_y = 20;
static const uint8_t space_between_aliens_x = 2;
static const uint8_t space_between_aliens_y = 16;
static const uint8_t aliens_velocity = 1;

static const uint16_t player_init_pos_x = 38;
static const uint16_t player_init_pos_y = 420;
static const uint8_t number_of_player_shots = 5;
static const uint8_t player_velocity = 1;

//
extern volatile uint8_t shoot_button;
extern volatile uint8_t left_button;
extern volatile uint8_t right_button;

//Static functions
static void init_aliens(Object *aliens)
{
	for(uint8_t i = 0; i < number_of_aliens_rows; i++)
	{
		for(uint8_t j = 0; j < number_of_aliens_cols; j++)
		{
			(aliens + i * number_of_aliens_cols + j)->type = alien_type;
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
		(shots + i)->type = player_shot_type;
		(shots + i)->is_alive = 0x00;
		(shots + i)->x_size = SHOT_SIZE_X;
		(shots + i)->y_size = SHOT_SIZE_Y;
	}
}

static void init_player(Object *player)
{
	player->is_alive = 0x01;
	player->relative_pos = 0;
	player->type = player_type;
	player->velocity = 0;
	player->x_size = PLAYER_SIZE_X;
	player->y_size = PLAYER_SIZE_Y;
	player->x_coordinate = player_init_pos_x;
	player->y_coordinate = player_init_pos_y;
	draw_player(player_init_pos_x, player_init_pos_y);
}

static void move(Object *objects, uint8_t number_of_objects)
{
	for(uint8_t i = 0; i < number_of_objects; i++)
	{
		erese_object((objects + i)->x_coordinate,(objects + i)->y_coordinate, (objects + i)->x_size, (objects + i)->y_size);
		switch ((objects + i)->type)
		{
			case alien_type:
				if((objects + i)->is_alive)
				{


					if(((objects + i)->relative_pos == -10) || ((objects + i)->relative_pos == 10))
					{
						(objects + i)->velocity *= -1;
					}
					(objects + i)->x_coordinate += (objects + i)->velocity;
					(objects + i)->relative_pos += (objects + i)->velocity;
					draw_alien1((objects + i)->x_coordinate, (objects + i)->y_coordinate);
				}
				break;
			case player_type:
					if (left_button)
					{
						objects->x_coordinate += objects->relative_pos > -20 ? -1 : 0;
						objects->relative_pos += objects->relative_pos > -20 ? -1 : 0;
					}
					else if (right_button)
					{
						objects->x_coordinate += objects->relative_pos < 20 ? 1 : 0;
						objects->relative_pos += objects->relative_pos < 20 ? 1 : 0;
					}
					draw_player(objects->x_coordinate, objects->y_coordinate);
				break;
			case player_shot_type:
				if((objects + i)->is_alive)
				{
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

	uint32_t time_alien_move;
	uint32_t time_player_move;
	uint32_t time_shots_move;
	Object aliens[number_of_aliens];
	Object player_shots[number_of_player_shots];
	Object player;


	init_aliens(aliens);
	init_player(&player);

	while(!shoot_button){}
	time_alien_move = get_time();
	time_player_move = get_time();
	time_shots_move = get_time();
	while(1)
	{

		if ((get_time() - time_alien_move) >= 500)
		{
			move(aliens, number_of_aliens);
			time_alien_move = get_time();
		}
		if ((get_time() - time_player_move) >= 150)
		{
			move(&player, 1);
			time_player_move = get_time();
		}
		if ((get_time() - time_shots_move) >= 100)
		{
			move(player_shots, number_of_player_shots);
			time_shots_move = get_time();
		}
	}

}

