/*
 *	bullet.h
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#ifndef _BULLET_H_
#define _BULLET_H_

void bullet_Cleanup();
char bullet_Fire();
void bullet_Make();
char bullet_Position();
void bullet_UpdatePosition();
char bullet_DidHitSomeone();
void bullet_KilledSomeone();
char bullet_GetSpriteID();

#endif //_BULLET_H_
