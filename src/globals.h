/*
 *	globals.h
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

/*-----------------------------------------------------------------------*/
#define	MAZE_WALL		160
#define	MAZE_COIN		'.'
#define	MAZE_CLEAR		' '
#define MAZE2SCREEN		0x3
#define MAZE_DIRECTIONS	4

#define	MONSTERS_MAX	5
#define NUM_HIGHSCORES	10
#define NUM_BULLETS		(MONSTERS_MAX-1)
#define NUM_BOOSTS		(MONSTERS_MAX-1)

/*-----------------------------------------------------------------------*/
extern char* gcp_theMaze;
extern int* gi_rowStart;
extern char *gcp_animData;
extern char gc_lives;
extern char gc_width;
extern char gc_height;
extern char gc_state;
extern char gc_numMonsters;
extern char gcp_monsterSpawnY[MONSTERS_MAX];
extern char gcp_monsterSpawnX[MONSTERS_MAX];
extern char gc_playerSpawnY;
extern char gc_playerSpawnX;
extern char gc_underPlayer;
extern char gc_bulletSpawnY;
extern char gc_bulletSpawnX;
extern char gc_bullets;
extern char gc_bulletActive;
extern char gc_boost;
extern char gc_boostActive;
extern int gi_levelScore;
extern int gi_points;
extern int gip_scores[NUM_HIGHSCORES];
extern char gcp_scoreLetters[NUM_HIGHSCORES][4];
extern char *gcp_scoreNames[NUM_HIGHSCORES];
extern unsigned int gi_score;

//---------------------------------------------------------------------------
extern const signed char gcp_nextPosX[5];
extern const signed char gcp_nextPosY[5];
extern const char gcp_oppositedir[MAZE_DIRECTIONS];

/*-----------------------------------------------------------------------*/
// Display Labels
extern char* gMainScroll;

extern char* gMainMenu[];
extern const char gMainMenuItems;

extern const char *gc_highscoreMessage[];

extern const char* gcp_instructions[];
extern const char gc_numInstructions;

#endif //_GLOBALS_H_