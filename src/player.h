/*
 *	player.h
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#ifndef _PLAYER_H_
#define _PLAYER_H_

void player_Cleanup();
void player_SetSpawn(char ny, char nx, char direction);
void player_Make();
char player_CanMove(char direction);
char player_Position(char nextDir);
void player_UpdatePosition();
char player_DidHitSomething();
char player_GetSpriteID();
void player_Get(char *nx, char *ny, char *direction);
char player_IsMoving();

#endif //_PLAYER_H_
