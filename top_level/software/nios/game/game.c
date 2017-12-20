/*
 * game.c
 *
 *  Created on: 17 gru 2017
 *      Author: caput
 */
#include "game.h"
#include "altera_avalon_uart_regs.h"
#include "../uart/uart.h"
#include <stdlib.h>



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
static const uint8_t number_of_aliens_rows = 6;
static const uint8_t number_of_aliens_cols = 8;
static const uint8_t number_of_aliens = 48;

static const uint8_t space_between_aliens_x = 2;
static const uint8_t space_between_aliens_y = 16;

static const uint8_t aliens_init_pos_x = 17;
static const uint8_t aliens_init_pos_y = 32;
static const uint8_t aliens_velocity = 1;

static const uint16_t player_init_pos_x = 38;
static const uint16_t player_init_pos_y = 420;

static const uint8_t player_velocity = 1;

static const int8_t shot_velocity = -16;
static const uint8_t number_of_player_shots = 5;
static const uint8_t number_of_alien_shots = 5;

//
volatile uint8_t left_button_board;
volatile uint8_t right_button_board;
volatile uint8_t shoot_button_board;

volatile uint8_t left_button_uart;
volatile uint8_t right_button_uart;
volatile uint8_t shoot_button_uart;

//Static functions
static void erase_object(Object *object)
{
	if (object->is_alive)
	{
		erase_draw(object->x_coordinate, object->y_coordinate, object->type);
	}
}


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
			draw_object((aliens + i * number_of_aliens_cols + j)->x_coordinate, (aliens + i * number_of_aliens_cols + j)->y_coordinate, alien_type);
		}
	}
}
//
static void init_shots(Object *shots, Object_type owner)
{
	for(uint8_t i = 0;i < number_of_player_shots; i++)
	{
		(shots + i)->type = shot_type;
		(shots + i)->is_alive = 0x00;
		(shots + i)->x_size = SHOT_SIZE_X;
		(shots + i)->y_size = SHOT_SIZE_Y;
		(shots + i)->velocity = (owner == player_type) ? shot_velocity : (-1) * shot_velocity;
		(shots + i)->relative_pos = 0;
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
	draw_object(player_init_pos_x, player_init_pos_y, player_type);
}

static void move(Object *objects, uint8_t number_of_objects)
{
	for(uint8_t i = 0; i < number_of_objects; i++)
	{
		erase_object((objects + i));
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
					draw_object((objects + i)->x_coordinate, (objects + i)->y_coordinate, (objects + i)->type);
				}
				break;
			case player_type:
					if (left_button_board || left_button_uart)
					{
						objects->x_coordinate += objects->relative_pos > -20 ? -1 : 0;
						objects->relative_pos += objects->relative_pos > -20 ? -1 : 0;
					}
					else if (right_button_board || right_button_uart)
					{
						objects->x_coordinate += objects->relative_pos < 20 ? 1 : 0;
						objects->relative_pos += objects->relative_pos < 20 ? 1 : 0;
					}
					draw_object(objects->x_coordinate, objects->y_coordinate, objects->type);
				break;
			case shot_type:
				if((objects + i)->is_alive)
				{
					if ((objects + i)->y_coordinate > 0 && (objects + i)->y_coordinate < MAX_Y)
					{
						(objects + i)->y_coordinate += (objects + i)->velocity;
						draw_object((objects + i)->x_coordinate, (objects + i)->y_coordinate,(objects + i)->type);
					}
					else
					{
						(objects + i)->is_alive = 0x00;
					}
				}
				break;
			default:
				break;
		}
	}
}

static void shoot(const Object* shooter, Object* possible_shots, const uint8_t number_of_possible_shots)
{
	if (shooter->is_alive)
	{
		for(uint8_t i = 0; i < number_of_possible_shots; i++)
		{
			if (!(possible_shots + i)->is_alive)
			{
				(possible_shots + i)->is_alive = 0x01;
				(possible_shots + i)->x_coordinate = shooter->x_coordinate + shooter->x_size / 4;
				(possible_shots + i)->y_coordinate = shooter->y_coordinate - ((shooter->type == player_type) ? (possible_shots + i)->y_size : -shooter->y_coordinate);
				(possible_shots + i)->relative_pos = 0;
				draw_object((possible_shots + i)->x_coordinate, (possible_shots + i)->y_coordinate, (possible_shots + i)->type);
				break;
			}
		}
	}
}


static void collision_detection(Object* shots, uint8_t number_of_shots, Object* targets, uint8_t number_of_targets)
{
	for (uint8_t i = 0; i < number_of_shots; i++)
	{
		for (uint8_t j = 0; (j < number_of_targets) && (shots + i)->is_alive; j++)
		{
			if ((shots + i)->is_alive && ((targets + j)->is_alive))
			{
				switch ((targets + j)->type) {
					case alien_type:
						if ((shots + i)->y_coordinate < (targets + j)->y_coordinate + (targets + j)->y_size)
						{
							if ((shots + i)->x_coordinate >= (targets + j)->x_coordinate)
							{
								if ((shots + i)->x_coordinate < ((targets + j)->x_coordinate) + (targets + j)->x_size)
								{
									erase_object(shots + i);
									erase_object(targets + j);
									(shots + i)->is_alive = 0x00;
									(targets + j)->is_alive = 0x00;
								}
							}
							else
							{
								if ((shots + i)->x_coordinate > ((targets + j)->x_coordinate - (shots + i)->x_size))
								{
									erase_object(shots + i);
									erase_object(targets + j);
									(shots + i)->is_alive = 0x00;
									(targets + j)->is_alive = 0x00;
								}
							}
						}
						break;
					case player_type:
						if ((shots + i)->y_coordinate > (targets + j)->y_coordinate + (targets + j)->y_size)
						{
							if ((shots + i)->x_coordinate >= (targets + j)->x_coordinate)
							{
								if ((shots + i)->x_coordinate < ((targets + j)->x_coordinate) + (targets + j)->x_size)
								{
									(shots + i)->is_alive = 0x00;
									(targets + j)->is_alive = 0x00;
								}
							}
							else
							{
								if ((shots + i)->x_coordinate > ((targets + j)->x_coordinate - (shots + i)->x_size))
								{
									(shots + i)->is_alive = 0x00;
									(targets + j)->is_alive = 0x00;
								}
							}
						}
						break;
					default:
						break;
				}
			}
		}
	}
}

static uint8_t count_aliens_left(const Object* aliens, uint8_t number_of_aliens)
{
	uint8_t result =0;
	for (uint8_t i = 0; i < number_of_aliens; i++)
	{
		if ((aliens + i)->is_alive)
		{
			result++;
		}
	}
	return result;
}

void game_run(void)
{

	uint32_t time_alien_move;
	uint32_t time_player_move;
	uint32_t time_player_shots_move;
	uint32_t time_alien_shots_move;
	Object aliens[number_of_aliens];
	Object aliens_shots[number_of_alien_shots];
	Object player_shots[number_of_player_shots];
	Object player;
	uint8_t aliens_alive;

	time_alien_move = get_time();
	time_player_move = get_time();
	time_player_shots_move = get_time();
	time_alien_shots_move = get_time();
	srand(get_time());
	while(1)
	{
		clear_screen();
		init_aliens(aliens);
		init_shots(player_shots, player_type);
		init_shots(aliens_shots, alien_type);
		init_player(&player);
		aliens_alive = count_aliens_left(aliens, number_of_aliens);
		while(!(shoot_button_board || shoot_button_uart)){}
		while(player.is_alive && aliens_alive)
		{

			if ((get_time() - time_alien_move) >= 400)
			{
				move(aliens, number_of_aliens);
				time_alien_move = get_time();
			}
			if ((get_time() - time_player_move) >= 150)
			{
				move(&player, 1);
				if (shoot_button_board || shoot_button_uart)
				{
					shoot(&player, player_shots, number_of_player_shots);

				}
				time_player_move = get_time();
			}
			if ((get_time() - time_player_shots_move) >= 100)
			{
				move(player_shots, number_of_player_shots);
				collision_detection(player_shots, number_of_player_shots, aliens, number_of_aliens);
				time_player_shots_move = get_time();
			}
			if ((get_time() - time_alien_shots_move) >= 120)
			{
				uint8_t random_alien = (uint8_t)(rand() % number_of_aliens);
				shoot(&aliens[random_alien], aliens_shots, number_of_alien_shots);
				move(aliens_shots, number_of_alien_shots);
				collision_detection(aliens_shots, number_of_alien_shots, &player, 1);
				time_alien_shots_move = get_time();
			}
			aliens_alive = count_aliens_left(aliens, number_of_aliens);
		}
		while(!(shoot_button_board || shoot_button_uart)){}
	}
}

