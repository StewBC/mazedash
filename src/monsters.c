/*
 *	monsters.c
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "monsters.h"
#include "sprites.h"

//---------------------------------------------------------------------------
static char sc_monsterX[MONSTERS_MAX];
static char sc_monsterY[MONSTERS_MAX];
static signed char sc_monsterDX[MONSTERS_MAX];
static signed char sc_monsterDY[MONSTERS_MAX];
static char sc_monsterSpriteID[MONSTERS_MAX] = {0xff,0xff,0xff,0xff,0xff};

//---------------------------------------------------------------------------
void monsters_Cleanup(char playerDied)
{
	char i;
	
	for(i=0;i<gc_numMonsters;++i)
	{
		sprite_Hide(sc_monsterSpriteID[i]);
		sprite_Release(sc_monsterSpriteID[i]);
	}
		
	if(!playerDied)
		gc_numMonsters = 0;
}

//---------------------------------------------------------------------------
void monsters_AddSpawn(char ny, char nx)
{
	if(nx-1 <= gc_playerSpawnX && nx+1 >= gc_playerSpawnX && ny-1 <= gc_playerSpawnY && ny+1 >= gc_playerSpawnY)
		return;
		
	if(gc_numMonsters < MONSTERS_MAX)
	{
		gcp_monsterSpawnY[gc_numMonsters] = ny;
		gcp_monsterSpawnX[gc_numMonsters] = nx;
		++gc_numMonsters;
	}
	else
	{
		if(rand() % 100 > 80)
		{
			char pos = rand() % MONSTERS_MAX;
			
			gcp_monsterSpawnY[pos] = ny;
			gcp_monsterSpawnX[pos] = nx;
		}
	}
}

//---------------------------------------------------------------------------
void monsters_Make()
{
	char i;
	
	for(i=0; i < gc_numMonsters; ++i)
	{
		if(0xff == (sc_monsterSpriteID[i] = sprite_Get()))
		{
			gc_numMonsters = i-1;
			return;
		}
		
		sc_monsterX[i] = gcp_monsterSpawnX[i];
		sc_monsterY[i] = gcp_monsterSpawnY[i] ;

		sc_monsterDX[i] = rand() % 2 ? -1 : 1;
		sc_monsterDY[i] = rand() % 2 ? -1 : 1;
		
		sprite_SetPosition(sc_monsterSpriteID[i], sc_monsterX[i], MAZE2SCREEN+sc_monsterY[i]);
		sprite_SetDirection(sc_monsterSpriteID[i], 0);
		sprite_Show(sc_monsterSpriteID[i]);
	}
}

//---------------------------------------------------------------------------
void monsters_Position()
{
	char i, nx, ny, block;
	
	for(i = 0; i < gc_numMonsters; ++i)
	{
		nx = sc_monsterX[i] + sc_monsterDX[i];
		ny = sc_monsterY[i] + sc_monsterDY[i];
		block = gcp_theMaze[gi_rowStart[ny]+nx];
		if(MAZE_WALL == block)
		{
			if(MAZE_WALL == gcp_theMaze[gi_rowStart[ny]+sc_monsterX[i]])
			{
				if(MAZE_WALL == gcp_theMaze[gi_rowStart[sc_monsterY[i]]+nx])
				{
					nx = sc_monsterX[i];
					ny = sc_monsterY[i];               
					sc_monsterDX[i] = rand()%2 ? -1 : 1;
					sc_monsterDY[i] = rand()%2 ? -1 : 1;
				}
				else
				{
					ny = sc_monsterY[i];
					sc_monsterDY[i] = rand()%2 ? -1 : 1;
				}
			}
			else
			{
				nx = sc_monsterX[i];
				sc_monsterDX[i] = rand()%2 ? -1 : 1;
			}
		}

		block = 4;
		if(nx != sc_monsterX[i])
		{
			if(nx < sc_monsterX[i])
				block = 3;
			else
				block = 1;
		}
		else if(ny != sc_monsterY[i])
		{
			if(ny < sc_monsterY[i])
				block = 0;
			else
				block = 2;
		}
		
		sc_monsterX[i] = nx;
		sc_monsterY[i] = ny;

		sprite_SetTargetPos(sc_monsterSpriteID[i], nx, MAZE2SCREEN+ny);
		if(4 != block)
			sprite_SetDirection(sc_monsterSpriteID[i], block);
	}
}

//---------------------------------------------------------------------------
void monsters_UpdatePositions()
{
	char i;
	
	for(i = 0; i < gc_numMonsters; ++i)
	{
		if(sprite_MoveSprite(sc_monsterSpriteID[i], 0))
			sprite_NextFrame(sc_monsterSpriteID[i]);
	}
}

//---------------------------------------------------------------------------
char monsters_DidHitMonster(char bulletSpriteID, char isBullet)
{
	char i, by, my;
	int bx, mx;

	sprite_GetCoordinates(bulletSpriteID, &bx, &by);
	
	for(i = 0; i < gc_numMonsters; ++i)
	{
		if(sprite_IsVisible(sc_monsterSpriteID[i]) && sprite_GetCollisionState(sc_monsterSpriteID[i]))
		{
			sprite_GetCoordinates(sc_monsterSpriteID[i], &mx, &my);
			if(mx <= bx+8 && mx+8 >= bx && my <= by+8 && my+8 >= by)
			{
				if(isBullet)
					sprite_Hide(sc_monsterSpriteID[i]);
				return 1;
			}
		}
	}
	return 0;
}
