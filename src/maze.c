/*
 *	maze.c
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
#include "plat.h"

//---------------------------------------------------------------------------
static char sc_cpx;
static char sc_cpy;
static char sc_dir;
static int si_offset;
static char sc_marchLength;
static const char scp_nextdir[MAZE_DIRECTIONS] = {1,2,3,0};
static const char scp_prevdir[MAZE_DIRECTIONS] = {3,0,1,2};
static const signed char scp_nextx[MAZE_DIRECTIONS] = {0,1,0,-1};
static const signed char scp_nexty[MAZE_DIRECTIONS] = {-1,0,1,0};
static const char scp_lookupindex[MAZE_DIRECTIONS] = {2,4,6,0};
static signed char scp_nextoffset[MAZE_DIRECTIONS] = {0,1,0,-1};
static signed char scp_lookup[11] = {10,9,-1,-11,-10,-9,1,11,10,9,-1};

//---------------------------------------------------------------------------
void maze_Init()
{
	scp_nextoffset[0] = -gc_width;
	scp_nextoffset[2] = gc_width;
	
	scp_lookup[0] = gc_width;
	scp_lookup[1] = gc_width-1;
	scp_lookup[3] = -(gc_width+1);
	scp_lookup[4] = -gc_width;
	scp_lookup[5] = -(gc_width-1);
	scp_lookup[7] = gc_width+1;
	scp_lookup[8] = gc_width;
	scp_lookup[9] = gc_width-1;
	
	sc_marchLength = 0;
}

//---------------------------------------------------------------------------
char maze_CheckMove()
{
	int po;
	char i, ix;
	char vc = 1;
	char nx = sc_cpx + scp_nextx[sc_dir], ny = sc_cpy + scp_nexty[sc_dir];
	
	if(!nx || gc_width-1 == nx || !ny || gc_height-1 == ny)
		return 0;
	
	po = si_offset + scp_nextoffset[sc_dir];
	ix = scp_lookupindex[sc_dir];

	if(MAZE_DIRECTIONS > gcp_theMaze[po])
	{
		vc = 0;
	}
	else
	{
		for(i = 0; i <= MAZE_DIRECTIONS; i++)
		{
			if(MAZE_DIRECTIONS > gcp_theMaze[po + scp_lookup[ i + ix ]])
			{
				vc = 0;
				break;
			}
		}
	}

	return vc;
}

//---------------------------------------------------------------------------
void SmashHole()
{
	char i, ix, nx, ny;
	int po;

	ix = scp_lookupindex[sc_dir];
	
	nx = 0;
	for(i = 0; i <= 4; i++)
	{
		if(MAZE_DIRECTIONS >= (gcp_theMaze[si_offset + scp_lookup[ i + ix ]]))
			++nx;
	}
	if(nx >= 2)
		return;

	po = si_offset + scp_nextoffset[sc_dir];

	for(i = 0; i <= 4; i++)
	{
		if(MAZE_DIRECTIONS > (gcp_theMaze[po + scp_lookup[ i + ix ]]))
		{
			nx = sc_cpx + scp_nextx[sc_dir];
			ny = sc_cpy + scp_nexty[sc_dir];
		
			if(nx != 1 && nx != gc_width-2 && ny != 1 && ny != gc_height-1)
			{
				if(MAZE_DIRECTIONS <= gcp_theMaze[po])
				{
					gcp_theMaze[po] = MAZE_DIRECTIONS;
					monsters_AddSpawn(ny, nx);
				}
				break;
			}
		}
	}
}

//---------------------------------------------------------------------------
void maze_MakePath()
{
	char turns = 0, moves = 0;
	
	sc_cpx = (rand() % (gc_width-4)) + 2;
	sc_cpy = (rand() % (gc_height-4)) + 2;
	si_offset = (sc_cpy*gc_width)+sc_cpx;
	sc_dir = rand() % MAZE_DIRECTIONS;

	gcp_theMaze[si_offset] = MAZE_DIRECTIONS;

	player_SetSpawn(sc_cpy, sc_cpx, sc_dir);
	
	do
	{
		do
		{
			if(!maze_CheckMove())
			{
				sc_dir = scp_nextdir[sc_dir];
				if(MAZE_DIRECTIONS == ++turns)
				{
					sc_dir = gcp_theMaze[si_offset];
					if(STATE_ESCAPE != gc_state)
					{
						if(moves > 1)
							SmashHole();
					}
					else
						monsters_AddSpawn(sc_cpy, sc_cpx);
					
					sc_dir = gcp_oppositedir[sc_dir];
					si_offset += scp_nextoffset[sc_dir];
					sc_cpx += scp_nextx[sc_dir];
					sc_cpy += scp_nexty[sc_dir];
					if(MAZE_DIRECTIONS == gcp_theMaze[si_offset])
						return;
					turns = 0;
					sc_dir = scp_nextdir[sc_dir];
				}
			}
			else
				break;
		} while(1);
	
		turns = 0;

		si_offset += scp_nextoffset[sc_dir];
		sc_cpx += scp_nextx[sc_dir];
		sc_cpy += scp_nexty[sc_dir];

		gcp_theMaze[si_offset] = sc_dir;

		if(++moves < 3)
			continue;
			
		moves = rand() % 3;
		if(moves)
		{
			if(1 == moves)
				sc_dir = scp_prevdir[sc_dir];
			else
				sc_dir = scp_nextdir[sc_dir];
		}
		moves = 0;
	} while(1);
}

//---------------------------------------------------------------------------
void maze_Finalise()
{
	int i, e;
	
	e = gc_width * (gc_height-1);
	gi_points = 0;
	for(i=gc_width+1;i<e;++i)
	{
		if(gcp_theMaze[i] <= MAZE_DIRECTIONS)
		{
			gcp_theMaze[i] = MAZE_COIN;
			++gi_points;
			plat_setColor(i, 7);
		}
	}
}

//---------------------------------------------------------------------------
void maze_MakeMaze()
{
	plat_Clear();
	
	memset(gcp_theMaze, MAZE_WALL, gc_width * gc_height);

	maze_MakePath();
	maze_Finalise();
}
