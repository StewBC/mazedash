/*
 *	frontend.h
 *	maze dash
 *
 *	Created by Stefan Wessels, May 2014.
 *
 */

#ifndef _FRONTEND_H_
#define _FRONTEND_H_

void frontend_ShowTitle(char vertical);
void frontend_ShowGameOver();
void frontend_ShowHighScores(char once);
void frontend_ShowInstructions();
void frontend_ShowHud();
void frontend_ShowBoostLabel();
void frontend_UpdateHud();
void frontend_EnterName(char index);
void frontend_Pause();
char frontend_Menu(char **menuItems, char height, char *scroller);

#endif //_FRONTEND_H_