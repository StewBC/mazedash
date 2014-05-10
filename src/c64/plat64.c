/*
 *	plat64.c
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#include <string.h>
#include <c64.h>
#include <conio.h>
#include "../types.h"
#include "../globals.h"
#include "../plat.h"
#include "anims.h"
#include "platinc.h"

/*-----------------------------------------------------------------------*/
// System locations
#define VIC_BASE_RAM			(0x0000)
#define SCREEN_RAM				((char*)VIC_BASE_RAM+0x0400)

/*-----------------------------------------------------------------------*/
static int sc_firstLinePositions[SCREEN_HEIGHT];

/*-----------------------------------------------------------------------*/
// Called one-time to set up the platform (or computer or whatever)
void plat_Init()
{
	int i;
	
	clrscr();
	
	VIC.bgcolor0 = COLOR_RED;
	VIC.bordercolor = COLOR_RED;
	
	gcp_theMaze = (char*)(SCREEN_RAM+SCREEN_WIDTH);
	gc_width = SCREEN_WIDTH;
	gc_height = SCREEN_HEIGHT-2;
	
	gi_rowStart = sc_firstLinePositions;
	gi_rowStart[0] = 0;
	for(i=1;i<gc_height;++i)
		gi_rowStart[i] = gi_rowStart[i-1] + gc_width;
	
	gcp_animData = (char*)animations;
}

/*-----------------------------------------------------------------------*/
// This is not needed on the C64
void plat_UpdateScreen()
{
	static char timer = 0;
	
	while(!plat_TimeExpired(0xff, &timer));
}

/*-----------------------------------------------------------------------*/
void plat_Clear()
{
	textcolor(COLOR_GRAY3);
	clrscr();
}

/*-----------------------------------------------------------------------*/
void plat_setColor(int index, char colour)
{
	*(char*)(0xd828+index) = colour;
}

/*-----------------------------------------------------------------------*/
// Use timer B to time a duration
char plat_TimeExpired(unsigned int aTime, char *timerInit)
{
	if(!*timerInit || (CIA1.tb_lo < aTime))
	{
		*timerInit = 1;
		
		CIA1.crb &= 0xfe;
		CIA1.tb_lo = 0xff;
		CIA1.tb_hi = 0xff;
		CIA1.crb |= 0x41;
		
		return 1;
	}
	return 0;
}

/*-----------------------------------------------------------------------*/
int plat_ReadKeys(char blocking)
{
	char key = 0;
	int keyMask = 0;
	
	if(blocking || kbhit())
		key = cgetc();
	else
		return 0;
		
	switch(key)
	{
		case 73:		// Up
		case 145:		// Up
			keyMask |= INPUT_UP;
		break;

		case 76:
		case 29:		// Right
			keyMask |= INPUT_RIGHT;
		break;

		case 75:
		case 17:		// Down
			keyMask |= INPUT_DOWN;
		break;

		case 74:
		case 157:		// Left
			keyMask |= INPUT_LEFT;
		break;
		
		case 90:		// Boost
			keyMask |= INPUT_BOOST;
		break;

		case 88:		// Shoot
			keyMask |= INPUT_SHOOT;
		break;
		
		case 3:			// Esc
			keyMask |= INPUT_BACKUP;
		break;

		case 77:		// 'm' - Menu
			keyMask |= INPUT_MENU;
		break;

		case 80:		// 'p' - Pause
			keyMask |= INPUT_MENU;
		break;

		case 13:		// Enter
			keyMask |= INPUT_SELECT;
		break;
		
		// default:		// Debug - show key code
		// {
		// 	char s[] = "Key:000";
		// 	s[4] = (key/100)+'0';
		// 	key -= (s[4] - '0') * 100;
		// 	s[5] = (key/10)+'0';
		// 	s[6] = (key%10)+'0';
		// 	gotoxy(20,0);cprintf("%s",s);
		// }
		break;
	}
	
	return keyMask;
}

/*-----------------------------------------------------------------------*/
void plat_Shutdown()
{
	clrscr();
	
	__asm__("lda #142");
	__asm__("jsr $ffd2");
}
