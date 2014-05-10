/*
 *	player.c
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "globals.h"
#include "maze.h"
#include "player.h"
#include "monsters.h"
#include "sprites.h"

//---------------------------------------------------------------------------
static char sc_playerX;
static char sc_playerY;
static char sc_playerNextX;
static char sc_playerNextY;
static char sc_playerDirection;
static char sc_mazeDirection;
static char sc_playerSpriteID = 0xff;

//---------------------------------------------------------------------------
void player_Cleanup()
{
	if(0xff != sc_playerSpriteID)
	{
		sprite_Hide(sc_playerSpriteID);
		sprite_Release(sc_playerSpriteID);
	}
}

//---------------------------------------------------------------------------
void player_SetSpawn(char ny, char nx, char direction)
{
	gc_playerSpawnY = ny;
	gc_playerSpawnX = nx;
	sc_mazeDirection = direction;
	if(STATE_ESCAPE != gc_state)
		gc_bullets = 0;
	else
		gc_bullets = NUM_BULLETS;
		
	gc_boost = NUM_BOOSTS;
		
}

//---------------------------------------------------------------------------
void player_Make()
{
	if(0xff == (sc_playerSpriteID = sprite_Get()))
		return;
	
	sc_playerX = sc_playerNextX = gc_playerSpawnX;
	sc_playerY = sc_playerNextY = gc_playerSpawnY;
	sc_playerDirection = sc_mazeDirection;

	sprite_SetPosition(sc_playerSpriteID, sc_playerX, MAZE2SCREEN+sc_playerY);
	sprite_SetDirection(sc_playerSpriteID, sc_playerDirection);
	sprite_Show(sc_playerSpriteID);
}

//---------------------------------------------------------------------------
char player_CanMove(char direction)
{
	char nx, ny, block;
	
	nx = sc_playerX + gcp_nextPosX[direction];
	ny = sc_playerY + gcp_nextPosY[direction];

	block = gcp_theMaze[gi_rowStart[ny]+nx];
	
	if(MAZE_WALL == block)
		return 0;

	return 1;
}

//---------------------------------------------------------------------------
char player_Position(char nextDir)
{
	char rv = 0;
	
	sc_playerX = sc_playerNextX;
	sc_playerY = sc_playerNextY;

	gc_underPlayer = gcp_theMaze[gi_rowStart[sc_playerY]+sc_playerX];
	if(MAZE_COIN == gc_underPlayer)
	{
		gcp_theMaze[gi_rowStart[sc_playerY]+sc_playerX] = MAZE_CLEAR;
		++gi_levelScore;
	}

	if(4 == nextDir || !player_CanMove(nextDir))
	{
		if(4 == sc_playerDirection || !player_CanMove(sc_playerDirection))
		{
			sc_playerDirection = 4;
			return 0;
		}
	}
	else
	{
		sc_playerDirection = nextDir;
		rv = 1;
	}

	sc_playerNextX += gcp_nextPosX[sc_playerDirection];
	sc_playerNextY += gcp_nextPosY[sc_playerDirection];

	sprite_SetTargetPos(sc_playerSpriteID, sc_playerNextX, MAZE2SCREEN+sc_playerNextY);
	if(4 != sc_playerDirection)
		sprite_SetDirection(sc_playerSpriteID, sc_playerDirection);
		
	return rv;
}

//---------------------------------------------------------------------------
void player_UpdatePosition()
{
	if(sprite_MoveSprite(sc_playerSpriteID, 0))
		sprite_NextFrame(sc_playerSpriteID);
}

//---------------------------------------------------------------------------
char player_DidHitSomething()
{
	return sprite_GetCollisionState(sc_playerSpriteID);
}

//---------------------------------------------------------------------------
char player_GetSpriteID()
{
	return sc_playerSpriteID;
}

//---------------------------------------------------------------------------
void player_Get(char *nx, char *ny, char *direction)
{
	if(nx)
		*nx = sc_playerX;
	if(ny)
		*ny = sc_playerY;
	if(direction)
		*direction = sc_playerDirection;
}

//---------------------------------------------------------------------------
char player_IsMoving()
{
	return sc_playerDirection != 4;
}
