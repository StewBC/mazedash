/*
 *	globals.c
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#include "globals.h"

/*-----------------------------------------------------------------------*/
char* gcp_theMaze;
int* gi_rowStart;
char *gcp_animData;
char gc_lives;
char gc_width;
char gc_height;
char gc_state;
char gc_numMonsters;
char gcp_monsterSpawnY[MONSTERS_MAX];
char gcp_monsterSpawnX[MONSTERS_MAX];
char gc_playerSpawnY;
char gc_playerSpawnX;
char gc_underPlayer;
char gc_bulletSpawnY;
char gc_bulletSpawnX;
char gc_bullets;
char gc_bulletActive;
char gc_boost;
char gc_boostActive;
int gi_levelScore;
int gi_points;
int gip_scores[NUM_HIGHSCORES];
char gcp_scoreLetters[NUM_HIGHSCORES][4];
char *gcp_scoreNames[NUM_HIGHSCORES];
unsigned int gi_score;

//---------------------------------------------------------------------------
const signed char gcp_nextPosX[5] = {0,1,0,-1,0};
const signed char gcp_nextPosY[5] = {-1,0,1,0,0};
const char gcp_oppositedir[MAZE_DIRECTIONS] = {2,3,0,1};


/*-----------------------------------------------------------------------*/
char* gMainScroll = "Maze Dash by S. Wessels, 2014.  Game Design by R. Wessels.  A Wessels & Wessels Production.";

char* gMainMenu[] = {"   Select   ", "  Survival  ", "   Escape   ", "    Quit    ", 0};
const char gMainMenuItems = sizeof(gMainMenu)/sizeof(gMainMenu[0]);

const char *gc_highscoreMessage[] = {"  TOP DOG!", " TOP THREE", "  MADE IT"};
const char* gcp_instructions[] =
{
	"     Maze Dash      ",
	"                    ",
	"Object of the game: ",
	"                    ",
	"You play as secret  ",
	" agent Mr. White.   ",
	"                    ",
	"Your mission is to  ",
	" collect all the    ",
	" intelligence dots. ",
	" Don't get caught by",
	" the guards.        ",
	"                    ",
	"Shoot guards only   ",
	" when you have no   ",
	" other choice.  Use ",
	" boost to outrun the",
	" guards when        ",
	" possible.          ",
	"                    ",
	"You get 1 point for ",
	" every piece of     ",
	" intelligence       ",
	" gathered.          ",
	"                    ",
	"You get 10 points   ",
	" for every unused   ",
	" boost.             ",
	"                    ",
	"You get 100 points  ",
	" for every unused   ",
	" bullet.            ",
	"                    ",
	"Modes:              ",
	"                    ",
	" Survival - you     ",
	"  have no gun!  If  ",
	"  you do clear the  ",
	"  level, you get the",
	"  full no-shoot     ",
	"  bonus.            ",
	"                    ",
	" Escape - You have a",
	"  gun but the maze  ",
	"  has less escape   ",
	"  routes.           ",
	"                    ",
	" Keys:              ",
	"                    ",
	" i - Move up        ",
	" j - Move left      ",
	" l - Move right     ",
	" k - Move down      ",
	" z - Use boost      ",
	" x - Fire           ",
	" p - Pause toggle   ",
	"                    ",
};
const char gc_numInstructions = sizeof(gcp_instructions) / sizeof(gcp_instructions[0]);
