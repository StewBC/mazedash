/*
 *	sprites.h
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#ifndef _SPRITES_H_
#define _SPRITES_H_

char sprite_Get();
void sprite_Release(char id);
void sprite_Show(char id);
void sprite_Hide(char id);
char sprite_IsVisible(char id);
void sprite_SetPosition(char id, int x, char y);
void sprite_SetTargetPos(char id, int nx, char ny);
void sprite_SetDirection(char id, char direction);
char sprite_MoveSprite(char id, char force);
void sprite_NextFrame(char id);
void sprite_PrepCollisionState();
char sprite_GetCollisionState(char id);
void sprite_GetCoordinates(char id, int *sx, char *sy);
char sprite_GetFrame(char id);

#endif //_SPRITES_H_