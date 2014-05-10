/*
 *	bullet.c
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#include "globals.h"
#include "player.h"
#include "bullet.h"
#include "sprites.h"

//---------------------------------------------------------------------------
static char sc_bulletX;
static char sc_bulletY;
static char sc_bulletDirection;
static char sc_bulletSpriteID = 0xff;

//---------------------------------------------------------------------------
void bullet_Cleanup()
{
	if(0xff != sc_bulletSpriteID)
	{
		sprite_Hide(sc_bulletSpriteID);
		sprite_Release(sc_bulletSpriteID);
	}
}

//---------------------------------------------------------------------------
char bullet_Fire()
{
	char nx, ny, direction;
	
	player_Get(&nx, &ny, &direction);

	gc_bulletSpawnY = ny;
	gc_bulletSpawnX = nx;
	sc_bulletDirection = direction;
	if(4 == direction)
		return 0;
	
	bullet_Make();
	return 1;
	// return bullet_Position();
}

//---------------------------------------------------------------------------
void bullet_Make()
{
	if(0xff == sc_bulletSpriteID)
	{
		if(0xff == (sc_bulletSpriteID = sprite_Get()))
			return;
	}
	
	sc_bulletX = gc_bulletSpawnX;
	sc_bulletY = gc_bulletSpawnY;

	sprite_SetPosition(sc_bulletSpriteID, sc_bulletX, MAZE2SCREEN+sc_bulletY);
	sprite_SetDirection(sc_bulletSpriteID, 4);
	if(1 == sc_bulletDirection || 3 == sc_bulletDirection)
		sprite_NextFrame(sc_bulletSpriteID);
	sprite_Show(sc_bulletSpriteID);

	gc_bulletActive = 1;
}

//---------------------------------------------------------------------------
char bullet_CanMove(char direction)
{
	char nx, ny, block;
	
	nx = sc_bulletX + gcp_nextPosX[direction];
	ny = sc_bulletY + gcp_nextPosY[direction];

	block = gcp_theMaze[gi_rowStart[ny]+nx];
	
	if(MAZE_WALL == block)
		return 0;

	return 1;
}

//---------------------------------------------------------------------------
char bullet_Position()
{
	if(!bullet_CanMove(sc_bulletDirection))
	{
		sprite_Hide(sc_bulletSpriteID);
		gc_bulletActive = 0;

		return 0;
	}
	else
	{
		sc_bulletX += gcp_nextPosX[sc_bulletDirection];
		sc_bulletY += gcp_nextPosY[sc_bulletDirection];
		sprite_SetTargetPos(sc_bulletSpriteID, sc_bulletX, MAZE2SCREEN+sc_bulletY);

		return 1;
	}
}

//---------------------------------------------------------------------------
void bullet_UpdatePosition()
{
	sprite_MoveSprite(sc_bulletSpriteID, 0);
}

//---------------------------------------------------------------------------
char bullet_DidHitSomeone()
{
	return sprite_GetCollisionState(sc_bulletSpriteID);
}

//---------------------------------------------------------------------------
void bullet_KilledSomeone()
{
	sprite_Hide(sc_bulletSpriteID);
	gc_bulletActive = 0;
}

//---------------------------------------------------------------------------
char bullet_GetSpriteID()
{
	return sc_bulletSpriteID;
}
