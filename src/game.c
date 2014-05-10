/*
 *	game.c
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

#include "types.h"
#include "maze.h"
#include "player.h"
#include "monsters.h"
#include "bullet.h"
#include "game.h"
#include "frontend.h"
#include "sprites.h"
#include "plat.h"

/*-----------------------------------------------------------------------*/
#define	ANIM_LENGTH		8

static int si_keyMask;
static int si_cache_points;
static int *si_next_event;
static int si_event_monster;
static int si_event_player;
static int si_event_bullet;
static char sc_nextDir;
static char sc_monster_ticks;
static char sc_player_ticks;
static char sc_bullet_ticks;
static char sc_playerKilled;
static char sc_timer;
static char sc_demoSpriteID;
static char sc_demoToggle;
static const char sc_turnposition[4][2] = { {3,1}, {0,2}, {1,3}, {2,0}};

/*-----------------------------------------------------------------------*/
void game_Init()
{
	char i;
	
	maze_Init();
	
	for(i=0;i<NUM_HIGHSCORES;++i)
	{
		gip_scores[i] = 0;
		gcp_scoreNames[i] = &gcp_scoreLetters[i][0];
		strcpy(gcp_scoreNames[i], "RJW");
	}
	sc_demoSpriteID = 0xff;

	si_event_monster = 500;
	si_event_player = 500;
	si_event_bullet = 0;
	sc_nextDir = 4;
	sc_monster_ticks = ANIM_LENGTH;
	sc_player_ticks = ANIM_LENGTH;
	sc_bullet_ticks = ANIM_LENGTH;
}

/*-----------------------------------------------------------------------*/
void game_Run()
{
	gc_state = STATE_TITLE;
	si_cache_points = 0;
	while(STATE_QUIT != gc_state)
	{
		switch(gc_state)
		{
			case STATE_TITLE:
				frontend_ShowTitle(0);
				gc_state = STATE_SHOWSCORES;
				break;
			
			case STATE_SHOWSCORES:
				frontend_ShowHighScores(0);
				if(2 == ++si_cache_points)
					gc_state = STATE_INSTRUCTIONS;
				else
					gc_state = STATE_TITLE;
				break;
				
			case STATE_INSTRUCTIONS:
				frontend_ShowInstructions();
				gc_state = STATE_DEMO;
				break;
			
			case STATE_DEMO:
				game_Play();
				gc_state = STATE_TITLE;
				si_cache_points = 0;
				break;
			
			case STATE_GAMEOVER:
				if(gi_score > gip_scores[NUM_HIGHSCORES-1])
				{
					gip_scores[NUM_HIGHSCORES-1] = gi_score;
					strcpy(gcp_scoreNames[NUM_HIGHSCORES-1], "---");
					frontend_EnterName(game_SortScores());
				}
				frontend_ShowGameOver();
				gc_state = STATE_SHOWSCORES;
				break;
		}
		if(0 != (si_keyMask = plat_ReadKeys(0)))
		{
			frontend_ShowTitle(1);
			gc_state = frontend_Menu(gMainMenu, gMainMenuItems, gMainScroll);
			if(STATE_SURVIVAL == gc_state || STATE_ESCAPE == gc_state)
			{
				game_Play();
				si_cache_points = 0;
			}
		}
	}
}

/*-----------------------------------------------------------------------*/
char game_SortScores()
{
	char i, j, *cpSwap, index = NUM_HIGHSCORES - 1;
	int iSwap;
	
	for(i=1;i<NUM_HIGHSCORES;++i)
	{
		for(j=NUM_HIGHSCORES-1;j>=i;--j)
		{
			if(gip_scores[j] > gip_scores[j-1])
			{
				iSwap = gip_scores[j];
				gip_scores[j] = gip_scores[j-1];
				gip_scores[j-1] = iSwap;
				
				cpSwap = gcp_scoreNames[j];
				gcp_scoreNames[j] = gcp_scoreNames[j-1];
				gcp_scoreNames[j-i] = cpSwap;
				
				if(j == index)
					--index;
				else if(j-1 == index)
					++index;
			}
		}
	}
	return index;
}

/*-----------------------------------------------------------------------*/
void game_Play()
{
	gi_score = 0;
	if(STATE_DEMO == gc_state)
	{
		sc_demoToggle = 1;
		gc_lives = 1;
	}
	else
		gc_lives = 3;
		
	sc_playerKilled = 0;
	
	while(gc_lives)
	{
		if(!sc_playerKilled)
		{
			si_cache_points = gi_levelScore = 0;
			maze_MakeMaze();
		}
			
		player_Make();
		monsters_Make();
		frontend_ShowHud();
		
		if(STATE_DEMO == gc_state && 0xff == sc_demoSpriteID)
		{
			player_Get(0,0,&sc_nextDir);
			sc_demoSpriteID = sprite_Get();
			if(0xff != sc_demoSpriteID)
			{
				sprite_SetPosition(sc_demoSpriteID,19,MAZE2SCREEN);
				sprite_SetDirection(sc_demoSpriteID, 4);
				sprite_NextFrame(sc_demoSpriteID);				
				sprite_NextFrame(sc_demoSpriteID);				
				sprite_Show(sc_demoSpriteID);
			}
		}
		
		sc_timer = 0;
		
		sc_playerKilled = 0;	
		plat_TimeExpired(0xa0, &sc_timer);
		while(!plat_TimeExpired(0xa0, &sc_timer));
		
		while(1)
		{
			si_next_event = si_event_monster < si_event_player ? &si_event_monster : &si_event_player;
			while(*si_next_event)
			{
				--si_event_monster;
				--si_event_player;
				if(1 < si_event_bullet)
				{
					--si_event_bullet;
				}
				else if(1 == si_event_bullet)
					break ;
			};
			
			if(INPUT_ACTION & si_keyMask || gi_levelScore != si_cache_points)
			{
				frontend_UpdateHud();
				++gi_score;
				si_cache_points = gi_levelScore;
			}
				
			if(INPUT_PAUSE == (si_keyMask = plat_ReadKeys(0)))
				frontend_Pause();
				
			if( gi_points == gi_levelScore)
				break;
				
			if(STATE_DEMO == gc_state)
			{
				if(si_keyMask)
				{
					gc_lives = 0;
					break;
				}
			}
			
			if(!player_IsMoving() && (si_keyMask & INPUT_MOTION))
			{
				si_event_player = 0;
				sc_player_ticks = 1;
			}
			
			if(si_keyMask & INPUT_UP)
				sc_nextDir = 0;
			if(si_keyMask & INPUT_RIGHT)
				sc_nextDir = 1;
			if(si_keyMask & INPUT_DOWN)
				sc_nextDir = 2;
			if(si_keyMask & INPUT_LEFT)
				sc_nextDir = 3;
			if(si_keyMask & INPUT_BOOST)
			{
				if(gc_boost && !gc_boostActive)
				{
					--gc_boost;
					gc_boostActive = 20 * ANIM_LENGTH;
					frontend_ShowBoostLabel();
				}
			}
			if(si_keyMask & INPUT_SHOOT)
			{
				if(gc_bullets && !gc_bulletActive)
				{
					if(bullet_Fire())
					{
						--gc_bullets;
						si_event_bullet = 1;
					}
				}
			}
			
			if(1 == si_event_bullet)
			{
				bullet_UpdatePosition();
				sprite_PrepCollisionState();
				if(bullet_DidHitSomeone() && monsters_DidHitMonster(bullet_GetSpriteID(), 1))
				{
					bullet_KilledSomeone();
					si_event_bullet = 0;
				}
				else
				{
					si_event_bullet = 10;
					if(!--sc_bullet_ticks)
					{
						if(!bullet_Position())
							si_event_bullet = 0;
						sc_bullet_ticks = ANIM_LENGTH;
					}
				}
			}
			if(!si_event_monster)
			{
				monsters_UpdatePositions();
				si_event_monster = 150;
				if(!--sc_monster_ticks)
				{
					monsters_Position();
					sc_monster_ticks = ANIM_LENGTH;

					if(STATE_DEMO == gc_state)
					{
						sc_demoToggle = 1 - sc_demoToggle;
						if(sc_demoToggle)
							sprite_Show(sc_demoSpriteID);
						else
							sprite_Hide(sc_demoSpriteID);
					}
				}
			}
			if(!si_event_player)
			{
				si_event_player = gc_underPlayer == MAZE_COIN ? 160 : 100;
				if(gc_boostActive)
				{
					if(--gc_boostActive)
						si_event_player -= 20;
					else
					{
						frontend_ShowBoostLabel();
						frontend_UpdateHud();
					}
				}
				player_UpdatePosition();
				sprite_PrepCollisionState();
				if(player_DidHitSomething() && monsters_DidHitMonster(player_GetSpriteID(), 0))
				{
					sc_playerKilled = 1;
					break;
				}
				else if(!--sc_player_ticks)
				{
					if(STATE_DEMO == gc_state)
					{
						char turn_count = 0;
						char turn_offset = rand() % 2;
						char turn_nextdir = sc_nextDir;

						if(player_CanMove(sc_turnposition[sc_nextDir][turn_offset]))
							turn_nextdir = sc_turnposition[sc_nextDir][turn_offset];
						while(!player_Position(turn_nextdir) && ++turn_count < 3)
						{
							turn_nextdir = sc_turnposition[sc_nextDir][turn_offset];
							if(++turn_offset > 1)
								turn_offset = 0;
						}
						if(3 != turn_count)
							sc_nextDir = turn_nextdir;
						else
							sc_nextDir = gcp_oppositedir[sc_nextDir];
						
					}
					else
					{
						if(player_Position(sc_nextDir))
							sc_nextDir = 4;
					}
						
					sc_player_ticks = ANIM_LENGTH;
				}
			}
		}
		player_Cleanup();
		monsters_Cleanup(sc_playerKilled);
		bullet_Cleanup();
		if(sc_playerKilled)
			--gc_lives;
		else
		{
			gi_score += gc_boost * 10;
			if(STATE_ESCAPE != gc_state)
				gi_score += NUM_BULLETS * 100;
			else
				gi_score += gc_bullets * 100;
			
			gc_boostActive = 0;
			gc_bulletActive = 0;
		}
	}
	
	if(STATE_DEMO == gc_state)
	{
		if(0xff != sc_demoSpriteID)
		{
			sprite_Hide(sc_demoSpriteID);
			sprite_Release(sc_demoSpriteID);
			sc_demoSpriteID = 0xff;
		}	
		gc_state = STATE_TITLE;
	}
	else
		gc_state = STATE_GAMEOVER;
}
