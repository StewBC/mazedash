/*
 *	sprites.c
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#include <c64.h>
#include "../types.h"
#include "../globals.h"
#include "../sprites.h"

//---------------------------------------------------------------------------
#define MAX_SPRITES		8
#define	ANIM_FRAMES		5
#define SPRITE_DATA		0x7f8
#define	SPRITE_LEFT		(31-7)
#define	SPRITE_TOP		34

//---------------------------------------------------------------------------
static char sc_spriteInUse[MAX_SPRITES];
static int si_spriteX[MAX_SPRITES];
static char sc_spriteY[MAX_SPRITES];
static int si_spriteDestX[MAX_SPRITES];
static char sc_spriteDestY[MAX_SPRITES];
static char sc_spriteFrame[MAX_SPRITES];
static char sc_spriteAnim[MAX_SPRITES];
static char sc_collisionRegister;
static const char scc_spriteNextFrame[] = {0,1,2,1,0,3,4,3};

static const char sprite_colours[MAX_SPRITES] = 
	{COLOR_WHITE, COLOR_BLACK, COLOR_CYAN, COLOR_YELLOW, COLOR_LIGHTGREEN, COLOR_GREEN, COLOR_BLACK, COLOR_BLACK};

//---------------------------------------------------------------------------
char sprite_Get()
{
	char i;
	
	for(i=0;i<MAX_SPRITES;++i)
	{
		if(!sc_spriteInUse[i])
		{
			sc_spriteInUse[i] = 1;
			sc_spriteFrame[i] = sc_spriteAnim[i] = 0;
			VIC.spr_color[i] = sprite_colours[i];

			return i;
		}
	}
	return -1;
}

//---------------------------------------------------------------------------
void sprite_Release(char id)
{
	sc_spriteInUse[id] = 0;
}

//---------------------------------------------------------------------------
void sprite_Show(char id)
{
	VIC.spr_ena |= SET_BIT(id);
}

//---------------------------------------------------------------------------
void sprite_Hide(char id)
{
	VIC.spr_ena &= ~SET_BIT(id);
}

//---------------------------------------------------------------------------
char sprite_IsVisible(char id)
{
	if(VIC.spr_ena & SET_BIT(id))
		return 1;
		
	return 0;
}

//---------------------------------------------------------------------------
void sprite_SetPosition(char id, int x, char y)
{
	si_spriteDestX[id] = si_spriteX[id] = (x*8) + SPRITE_LEFT;
	sc_spriteDestY[id] = sc_spriteY[id] = (y*8) + SPRITE_TOP;
	sprite_MoveSprite(id, 1);
}

//---------------------------------------------------------------------------
void sprite_SetTargetPos(char id, int x, char y)
{
	si_spriteDestX[id] = (x*8) + SPRITE_LEFT;
	sc_spriteDestY[id] = (y*8) + SPRITE_TOP;
}

//---------------------------------------------------------------------------
void sprite_SetDirection(char id, char direction)
{
	sc_spriteAnim[id] = ((int)gcp_animData / 64) + (ANIM_FRAMES * direction);
	sc_spriteFrame[id] = 0;
	*(char*)(SPRITE_DATA+id) = sc_spriteAnim[id];
}

//---------------------------------------------------------------------------
char sprite_MoveSprite(char id, char force)
{
	char result = force;
	
	if(si_spriteDestX[id] != si_spriteX[id])
	{
		if(si_spriteDestX[id] > si_spriteX[id])
			++si_spriteX[id];
		else
			--si_spriteX[id];
			
		result = 1;
	}

	if(sc_spriteDestY[id] != sc_spriteY[id])
	{
		if(sc_spriteDestY[id] > sc_spriteY[id])
			++sc_spriteY[id];
		else
			--sc_spriteY[id];
			
		result = 1;
	}

	if(result)
	{
		if(si_spriteX[id] > 255)
			*(char*)(0xD010) |= SET_BIT(id);
		else
			*(char*)(0xD010) &= ~SET_BIT(id);
	
		VIC.spr_pos[id].x = (si_spriteX[id] & 0xff);
		VIC.spr_pos[id].y = sc_spriteY[id];
	
		return 1;
	}
	
	return 0;
}

//---------------------------------------------------------------------------
void sprite_NextFrame(char id)
{
	if(++sc_spriteFrame[id] > 7)
		sc_spriteFrame[id] = 0;
	*(char*)(SPRITE_DATA+id) = sc_spriteAnim[id] + scc_spriteNextFrame[sc_spriteFrame[id]];
}

//---------------------------------------------------------------------------
void sprite_PrepCollisionState()
{
	sc_collisionRegister = VIC.spr_coll;
}

//---------------------------------------------------------------------------
char sprite_GetCollisionState(char id)
{
	return sc_collisionRegister & SET_BIT(id);
}

//---------------------------------------------------------------------------
void sprite_GetCoordinates(char id, int *sx, char *sy)
{
	*sx = VIC.spr_pos[id].x;
	*sy = VIC.spr_pos[id].y;
	
	if(*(char*)(0xD010) & SET_BIT(id))
		*sx += 256;
}

//---------------------------------------------------------------------------
char sprite_GetFrame(char id)
{
	return sc_spriteFrame[id];
}
