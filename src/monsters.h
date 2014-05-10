/*
 *	monsters.h
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#ifndef _MONSTERS_H_
#define _MONSTERS_H_

void monsters_Cleanup(char playerDied);
void monsters_Make();
void monsters_AddSpawn(char ny, char nx);
void monsters_Position();
void monsters_UpdatePositions();
char monsters_DidHitMonster(char bulletSpriteID, char isBullet);

#endif //_MONSTERS_H_