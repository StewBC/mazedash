/*
 *	plat.h
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#ifndef _PLAT_H_
#define _PLAT_H_

void plat_Init();
void plat_UpdateScreen();
void plat_Clear();
void plat_setColor(int index, char colour);
char plat_TimeExpired(unsigned int aTime, char *timerInit);
int plat_ReadKeys(char blocking);
void plat_Shutdown();

#endif //_PLAT_H_