/*
 *	main.c
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#include "game.h"
#include "plat.h"

/*-----------------------------------------------------------------------*/
// Internal function Prototype
void main_Init();
void main_Loop();

/*-----------------------------------------------------------------------*/
int main()
{
	main_Init();
	game_Run();
	plat_Shutdown();
	
	return 0;
}

/*-----------------------------------------------------------------------*/
void main_Init()
{
	plat_Init();
	game_Init();
}

