#ifndef _REVERSI_H_ 
#define _REVERSI_H_

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <dos.h>
#include <conio.h>

#define EMPTY 0
#define BLA 2
#define WHI 1
#define STARTX 100
#define STARTY 80
#define SIDELENGTH 40 /*边长*/
#define RADIUS 18
#define MSIZE 8 /*鼠标大小*/

extern int color[9][9];
extern void far *imagep;
extern int mouseLeftFlag;
extern int mx;
extern int my;
extern int mousex;
extern int mousey;
extern int colornow;
extern int flag, bla, whi, count, stopcheck;
extern int inow, jnow, ijnowcolor;
extern char black[10];
extern char white[10];
extern int win;
	
void Welcome();
void InitializeGraph();

/*NewGame Functions*/
void NewGame();
void drawTable();
void drawBlock(int x, int y);
void initializeTable();
void drawChessman(int i, int j, int color);
void drawChessnow();

/*Gaming Functions*/
int Gaming();
void MusicWithWelcome();
void ChessBlaSound();
void ChessWhiSound();
void WarningSound();
void initMouse();
void getMousePosition(int *x, int *y);
int leftPress();

int CheckWin();
int SureExit();

/*Change  Functions*/
int change(int x, int y);
int up(int x,int y);
int left(int x,int y);
int down(int x,int y);
int right(int x,int y);
int leftup(int x,int y);
int leftdown(int x,int y);
int rightdown(int x,int y);
int rightup(int x,int y);

/*determine  Functions*/
int determint(int x, int y);
int upd(int x,int y);
int leftd(int x,int y);
int downd(int x,int y);
int rightd(int x,int y);
int leftupd(int x,int y);
int leftdownd(int x,int y);
int rightdownd(int x,int y);
int rightupd(int x,int y);

void printScore();
#endif