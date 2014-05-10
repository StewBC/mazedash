/*
 *	frontend.c
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <c64.h>
#include <conio.h>
#include "../types.h"
#include "../globals.h"
#include "../frontend.h"
#include "../plat.h"
#include "platinc.h"

/*-----------------------------------------------------------------------*/
static char sc_ammoPos;
static char sc_boostPos;
static char sc_livesPos;
static char sc_scorePos;
static char sc_counter;

const static char *cscp_maze[] = 
{
	{"M   M   A   ZZZZZ EEEEE"},
	{"MM MM  A A      Z E    "},
	{"M M M A   A    Z  E    "},
	{"M   M AAAAA   Z   EEEE "},
	{"M   M A   A  Z    E    "},
	{"M   M A   A Z     E    "},
	{"M   M A   A ZZZZZ EEEEE"},
};

const static char *cscp_dash[] =
{
	{"DDD     A    SSSS H   H"},
	{"D  D   A A  S     H   H"},
	{"D   D A   A S     H   H"},
	{"D   D AAAAA  SSS  HHHHH"},
	{"D   D A   A     S H   H"},
	{"D  D  A   A     S H   H"},
	{"DDD   A   A SSSS  H   H"},
};

const static char *cscp_game[] =
{
	{" GGG    A   M   M EEEEE"},
	{"G   G  A A  MM MM E    "},
	{"G     A   A M M M E    "},
	{"G  GG AAAAA M   M EEEE "},
	{"G   G A   A M   M E    "},
	{"G   G A   A M   M E    "},
	{" GGG  A   A M   M EEEEE"},
};

const static char *cscp_over[] =
{
	{" OOO  V   V EEEEE RRRR "},
	{"O   O V   V E     R   R"},
	{"O   O V   V E     R   R"},
	{"O   O V   V EEEE  RRRR "},
	{"O   O  V V  E     R R  "},
	{"O   O  V V  E     R  R "},
	{" OOO    V   EEEEE R   R"},
};

/*-----------------------------------------------------------------------*/
void frontend_ShowLable(const char** label1, const char** label2, char *colours)
{
	char i, j;
	
	for(i=0;i<7;++i)
	{
		gotoxy(8,4+i);
		for(j=0;j<4;++j)
		{
			textcolor(colours[j]);
			cprintf("%.6s", label1[i]+(6*j));
		}
	}

	for(i=0;i<7;++i)
	{
		gotoxy(8,14+i);
		for(j=0;j<4;++j)
		{
			textcolor(colours[4+j]);
			cprintf("%.6s", label2[i]+(6*j));
		}
	}
}

/*-----------------------------------------------------------------------*/
void frontend_ShowTitle(char vertical)
{
	char i, j, k, colours[8];
	
	plat_Clear();
	sc_counter = 0;
		
	while((!kbhit() || vertical) && ++sc_counter < 20)
	{
		for(i=0;i<8;++i)
		{
			colours[i] = 1 + rand() % 15;
			if(COLOR_RED == colours[i])
				++colours[i];
		}

		if(vertical)
		{
			for(i=0;i<4;++i)
			{
				textcolor(colours[i]);
				for(j=0;j<5;++j)
				{
					gotoxy(2,22-(6*i+j));
					for(k=0;k<7;++k)
						cprintf("%c", cscp_maze[k][6*i+j]);
				}
			}

			for(i=0;i<4;++i)
			{
				textcolor(colours[4+i]);
				for(j=0;j<5;++j)
				{
					gotoxy(31,6*i+j);
					for(k=0;k<7;++k)
						cprintf("%c", cscp_dash[6-k][6*i+j]);
				}
			}
			break;
		}
		else
		{
			frontend_ShowLable(cscp_maze, cscp_dash, colours);
		}
	}
}

/*-----------------------------------------------------------------------*/
void frontend_ShowGameOver()
{
	char i, colours[8];
	
	plat_Clear();
	frontend_ShowHud();
	sc_counter = 0;
	
	while(0 != plat_ReadKeys(0));
	while(!kbhit() && ++sc_counter < 20)
	{
		for(i=0;i<8;++i)
		{
			colours[i] = 1 + rand() % 15;
			if(COLOR_RED == colours[i])
				++colours[i];
		}

		frontend_ShowLable(cscp_game, cscp_over, colours);
	}
}

/*-----------------------------------------------------------------------*/
void frontend_ShowHighScores(char once)
{
	char i, colour;
	
	frontend_ShowTitle(1);
	sc_counter = once ? 48 : 0;
	textcolor(COLOR_YELLOW);
	gotoxy((SCREEN_WIDTH/2)-6,((SCREEN_HEIGHT-NUM_HIGHSCORES)/2)-2);
	cprintf("HIGH SCORES");
	for(i=0;i<10;++i)
	{
		gotoxy(10,i+(SCREEN_HEIGHT-NUM_HIGHSCORES)/2);
		cprintf("%2d. ",1+i);
	}
	
	while(!kbhit() && ++sc_counter < 50)
	{
		for(i=0;i<NUM_HIGHSCORES;++i)
		{
			gotoxy(14,i+(SCREEN_HEIGHT-NUM_HIGHSCORES)/2);
			colour = 1 + rand() % 15;
			if(COLOR_RED == colour)
				++colour;
			textcolor(colour);
			cprintf("%05d - %s", gip_scores[i], gcp_scoreNames[i]);
		}
	}
}

/*-----------------------------------------------------------------------*/
void frontend_ShowInstructions()
{
	char currIns, currLine, startIns = 0, startLine = SCREEN_HEIGHT - 1, timer = 0;
	
	frontend_ShowTitle(1);
	sc_counter = 0;
	textcolor(COLOR_YELLOW);
	
	plat_TimeExpired(0xe0, &timer);
	while(!kbhit() && ++sc_counter < (gc_numInstructions + (SCREEN_HEIGHT-1)))
	{
		currIns = startIns;
		currLine = startLine;
		while(currLine < SCREEN_HEIGHT - 1)
		{
			gotoxy(10, currLine);
			cprintf("%s", gcp_instructions[currIns]);
			if(currIns < gc_numInstructions-1)
				++currIns;
			currLine += 1;
		}
		if(startLine > 1)
		{
			startLine -= 1;
		}
		else
		{
			if(startIns < gc_numInstructions-1)
				++startIns;
		}
		while(!plat_TimeExpired(0xe0, &timer));
	}
}

/*-----------------------------------------------------------------------*/
void frontend_ShowHud()
{
	gotoxy(1,0);
	textcolor(COLOR_WHITE);
	cprintf("Ammo: ");
	sc_ammoPos = wherex();

	frontend_ShowBoostLabel();
	sc_boostPos = wherex();
	
	gotoxy(sc_boostPos+2,0);
	textcolor(COLOR_GREEN);
	cprintf("Lives: ");
	sc_livesPos = wherex();

	gotoxy(sc_livesPos+2,0);
	textcolor(COLOR_YELLOW);
	cprintf("Score: ");
	sc_scorePos = wherex();
	
	frontend_UpdateHud();
}

/*-----------------------------------------------------------------------*/
void frontend_ShowBoostLabel()
{
	gotoxy(sc_ammoPos+2,0);
	if(gc_boostActive)
		textcolor(COLOR_LIGHTBLUE);
	else
		textcolor(COLOR_GRAY3);
	cprintf("Boost: ");
}

/*-----------------------------------------------------------------------*/
void frontend_UpdateHud()
{
	gotoxy(sc_ammoPos,0);
	textcolor(COLOR_WHITE);
	cprintf("%d", gc_bullets);

	gotoxy(sc_boostPos,0);
	if(gc_boostActive)
		textcolor(COLOR_LIGHTBLUE);
	else
		textcolor(COLOR_GRAY3);
	cprintf("%d", gc_boost);

	gotoxy(sc_livesPos,0);
	textcolor(COLOR_GREEN);
	cprintf("%d", gc_lives);

	gotoxy(sc_scorePos,0);
	textcolor(COLOR_YELLOW);
	cprintf("%05d", gi_score);
}

/*-----------------------------------------------------------------------*/
void frontend_EnterName(char index)
{
	char i, colour, pos = 0, key = 0;
	
	frontend_ShowHighScores(1);
	sc_counter = 0;
	
	while(0 != plat_ReadKeys(0));
	while(13 != key && ++sc_counter < 50)
	{
		textcolor(rand()%16);
		gotoxy((SCREEN_WIDTH/2)-6,((SCREEN_HEIGHT-NUM_HIGHSCORES)/2)-4);
		cprintf("%s",gc_highscoreMessage[index ? index < 3 ? 1 : 2 : 0]);
		for(i=0;i<NUM_HIGHSCORES;++i)
		{
			gotoxy(14,i+(SCREEN_HEIGHT-NUM_HIGHSCORES)/2);
			if(index == i)
				textcolor(COLOR_WHITE);
			else
			{
				colour = 2 + rand() % 15;
				if(COLOR_RED == colour)
					++colour;
				textcolor(colour);
			}
			cprintf("%05d - %s", gip_scores[i], gcp_scoreNames[i]);
		}
		if(kbhit())
		{
			if(13 != (key = cgetc()))
			{
				sc_counter = 0;
				if(20 == key)
				{
					if(pos)
						gcp_scoreNames[index][--pos] = '-';
				}
				else if(pos < 3)
				{
					gcp_scoreNames[index][pos++] = key;
				}
			}
		}
	}
}

/*-----------------------------------------------------------------------*/
void frontend_Pause()
{
	char timer = 0, colour;
	int key;
	
	gotoxy(0,0);
	cclear(SCREEN_WIDTH);

	plat_TimeExpired(0xa0, &timer);

	while(1)
	{
		gotoxy(SCREEN_WIDTH/2-2,0);
		colour = rand() % 16;
		if(COLOR_RED == colour)
			++colour;
		textcolor(colour);
		cprintf("Paused");

		while(INPUT_PAUSE != (key = plat_ReadKeys(0)) && !plat_TimeExpired(0xf0, &timer));
		if(INPUT_PAUSE == key)
			break;
	}
	gotoxy(0,0);
	cclear(SCREEN_WIDTH);
	frontend_ShowHud();
}

/*-----------------------------------------------------------------------*/
// Very simple menu with a heading and a scrolling banner as a footer
char frontend_Menu(char **menuItems, char height, char *scroller)
{
	static char *prevScroller, *pScroller, *pEnd;
	int keyMask;
	char i, j, sx, sy, numMenuItems, timerInit = 0, maxLen = 0;

	sc_counter = 150;
	
	// If the scroller message chages, cache the new one
	if(prevScroller != scroller)
	{
		prevScroller = scroller;
		pScroller = scroller;
		pEnd = scroller + strlen(scroller);
	}

	// Find the longest entry
	for(numMenuItems=0; menuItems[numMenuItems]; ++numMenuItems)
	{
		char len = strlen(menuItems[numMenuItems]);
		if(len > maxLen)
			maxLen = len;
	}
	
	// Centre on the screen
	sy = MAX_SIZE(0, (SCREEN_HEIGHT / 2) - (height / 2) - 1);
	sx = MAX_SIZE(0, (SCREEN_WIDTH / 2) - (maxLen / 2) - 1);
	maxLen = MIN_SIZE(SCREEN_WIDTH-2, maxLen);

	// Show the title
	textcolor(COLOR_GREEN);
	gotoxy(sx, sy);
	cprintf(" %.*s ",maxLen, menuItems[0]);
	
	// Leave a blank line
	textcolor(COLOR_BLACK);
	gotoxy(sx, ++sy);
	for(j=0; j<maxLen+2; ++j)
		cputc(' ');
	
	// Show all the menu items
	for(i=1; i<numMenuItems; ++i)
	{
		gotoxy(sx, sy+i);
		cprintf(" %.*s ",maxLen, menuItems[i]);
	}
	
	// Pad with blank lines to menu height
	for(;i<height;++i)
	{
		gotoxy(sx, sy+i);
		for(j=0; j<maxLen+2; ++j)
			cputc(' ');
	}

	// Select the first item
	i = 1;
	do
	{
		// Highlight the selected item
		gotoxy(sx, sy+i);
		textcolor(COLOR_WHITE);
		cprintf(">%.*s<",maxLen, menuItems[i]);
		textcolor(COLOR_BLACK);
		
		// Look for user input
		keyMask = plat_ReadKeys(0);
		if(keyMask)
			sc_counter = 150;
		
		if(keyMask & INPUT_MOTION)
		{
			// selection changes so de-highlight the selected item
			gotoxy(sx, sy+i);
			cprintf(" %.*s ",maxLen, menuItems[i]);
			
			// see if the selection goes up or down
			switch(keyMask & INPUT_MOTION)
			{
				case INPUT_UP:
					if(!--i)
						i = numMenuItems-1;
				break;
			
				case INPUT_DOWN:
					if(numMenuItems == ++i)
						i = 1;
				break;
			}
		}
		keyMask &= (INPUT_SELECT | INPUT_BACKUP);

		// Show the scroller
		gotoxy(sx,sy+height);
		textcolor(COLOR_CYAN);
		cprintf(" %.*s ",maxLen, pScroller);
		
		// Wrap the message if needed
		if((pEnd - pScroller) < maxLen-1)
		{
			gotoxy(sx+(pEnd-pScroller)+1,sy+height);
			cprintf(" %.*s ",maxLen-(pEnd - pScroller)-1, scroller);
		}

		// Only update the scrolling when needed
		if(plat_TimeExpired(SCROLL_SPEED, &timerInit))
		{
			--sc_counter;
			++pScroller;
			if(!*pScroller)
				pScroller = scroller;
		}
	} while(keyMask != INPUT_SELECT && keyMask != INPUT_BACKUP && sc_counter);

	// if backing out of the menu, return 0
	if(keyMask & INPUT_BACKUP || !sc_counter)
		return 0;
	
	// return the selection
	return i;
}
