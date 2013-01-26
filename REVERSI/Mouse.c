/*
 * Mouse.c
 *
 *  Created on: 2011-5-8
 *      Author: The_reAsOn
 */

#include "Reversi.h"
 
/*
 * Name: void InitMouse()
 * Parameter: None
 * Return: None
 * Function: Initialize mouse
 */
void initMouse(){
	union REGS ireg,oreg;
	int ret;
	struct SREGS s;
	ireg.x.ax = 0;
	int86(0x33,&ireg,&oreg);
	ret = oreg.x.ax;
	if(ret != -1)
	{
		printf("Mouse is not installed.");
		exit(1);
	}
}

/*
 * Name: void GetMousePosition(int *x, int *y)
 * Parameter: int *x, int *y   (coordinate of mouse)
 * Return: None
 * Function: get mouse location
 */
void getMousePosition(int *x, int *y){
	union REGS ireg,oreg;
	ireg.x.ax = 3;
	int86(0x33,&ireg,&oreg);
	*x = oreg.x.cx;
	*y = oreg.x.dx;
}

/*
 * Name: int leftPress()
 * Parameter: None
 * Return: 1-Press left 0-Not press
 * Function: Read left press
 */
int leftPress(){
	union REGS ireg,oreg;
	ireg.x.ax = 3;
	int86(0x33,&ireg,&oreg);
	delay(20000);
	if(oreg.x.bx&1)
		mouseLeftFlag = 1;
	return oreg.x.bx&1;
}
