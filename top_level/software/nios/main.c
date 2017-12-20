#include "system.h"

#include "timers/timers.h"
#include "uart/uart.h"

#include "drawing/drawing.h"
#include "game/game.h"


int main()
{ 
	init_uart();
	init_timers();

	while (1)
	{
		game_run();
	}

	return 0;
}

