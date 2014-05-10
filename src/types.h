/*
 *	types.h
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#ifndef _TYPES_H_
#define _TYPES_H_

/*-----------------------------------------------------------------------*/
#define SET_BIT(x)				(1<<(x))
#define MAX_SIZE(a,b)			(a)>(b)?(a):(b)
#define MIN_SIZE(a,b)			(a)<(b)?(a):(b)

/*-----------------------------------------------------------------------*/
#define INPUT_UP			SET_BIT(0)
#define INPUT_RIGHT			SET_BIT(1)
#define INPUT_DOWN			SET_BIT(2)
#define INPUT_LEFT			SET_BIT(3)
#define INPUT_BACKUP		SET_BIT(4)
#define INPUT_BOOST			SET_BIT(5)
#define INPUT_SHOOT			SET_BIT(6)
#define INPUT_SELECT		SET_BIT(7)
#define INPUT_MENU			SET_BIT(8)
#define INPUT_PAUSE			SET_BIT(8)
#define INPUT_MOTION		(INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT)
#define INPUT_ACTION		(INPUT_BOOST | INPUT_SHOOT)

/*-----------------------------------------------------------------------*/
enum
{
	STATE_TITLE,
	STATE_SURVIVAL,
	STATE_ESCAPE,
	STATE_QUIT,
	STATE_SHOWSCORES,
	STATE_INSTRUCTIONS,
	STATE_DEMO,
	STATE_GAMEOVER,
};

#endif //_TYPES_H_