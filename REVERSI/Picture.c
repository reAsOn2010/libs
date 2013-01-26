/*
 * Picture.c
 *
 *  Created on: 2011-5-8
 *      Author: The_reAsOn
 */
#include "Reversi.h"
/*
 * Name: void InitializeGraph()
 * Parameter: None
 * Return: None
 * Function: Initialize Graph mode
 */
void InitializeGraph(){
	int gdriver = VGA, gmode = VGAHI, errorcode;
	struct fillsettingstype save;
	initgraph(&gdriver, &gmode,"");

	/*--read the result of initializing--*/
	errorcode = graphresult();

	if(errorcode != grOk){
		printf("Graphics Error: %s\n",grapherrormsg(errorcode));
		printf("Press any key to halt:");
		getch();
		exit(1);
	}
	getfillsettings(&save);
}

/*
 * Name: void Welcome()
 * Parameter: None
 * Return: None
 * Function: Draw welcome screen
 */
void Welcome(){
	setcolor(WHITE);
	settextjustify(1,1);
	outtextxy(320,70,"Welcome to Reversi V1.0 Alpha");
	outtextxy(320,140,"Press any key to continue");
}

/*
 * Name: void DrawTable()
 * Parameter: None
 * Return: None
 * Function: Draw the Reversi table
 */

void drawTable(){
	int i, j;
	int xt, yt;
	for(i = 1; i < 9; i++){
		for(j = 1; j < 9; j++){
			xt = STARTX + j * SIDELENGTH;
			yt = STARTY + i * SIDELENGTH;
			drawBlock(xt, yt);
		}
	}
}

/*
 * Name: void DrawBlock()
 * Parameter: integer x, integer y, [coordinates of rectangle  (top left corner)]  &&  define SIDELENGTH
 * Return: None
 * Function: Draw the Reversi block
 */
void drawBlock(int x, int y){
	setcolor(EGA_MAGENTA);	
	setfillstyle(CLOSE_DOT_FILL,EGA_BROWN);
	bar(x, y, x+SIDELENGTH, y+SIDELENGTH);
	rectangle(x, y, x+SIDELENGTH, y+SIDELENGTH);
}	

/*
 * Name: void InitializeTable()
 * Parameter: None
 * Return: None
 * Function: Initialize the array table[9][9]
 */
void initializeTable(){
	int i, j;
	for(i = 1; i < 9; i++){
		for(j = 1; j < 9; j++){
			color[i][j] = EMPTY;
		}
	}
	color[4][4] = WHI;
	color[4][5] = BLA;
	color[5][4] = BLA;
	color[5][5] = WHI;
}

/*
 * Name; void drawChessman()
 * Parameter: integer x, integer y, [coordinate of chessman (center)]   integer color  &&  define RADIUS
 * Return: None
 * Function: Draw chessman  (Black or White)
 */
void drawChessman(int i, int j, int color){
	if(i != inow || j != jnow){		
		if(color == WHI){	
			setfillstyle(1,WHITE);
			setcolor(LIGHTGRAY);
			fillellipse(STARTX + j * SIDELENGTH + SIDELENGTH / 2, STARTY + i * SIDELENGTH + SIDELENGTH / 2, RADIUS,RADIUS);
		}
		else if(color == BLA){
			setfillstyle(1,BLACK);
			setcolor(DARKGRAY);
			fillellipse(STARTX + j * SIDELENGTH + SIDELENGTH / 2, STARTY + i * SIDELENGTH + SIDELENGTH / 2, RADIUS,RADIUS);
		}
	}
}

void drawChessnow(){
	int i, j, k;
	for(k = 1; k <= 9; k++){
		cleardevice();
		drawTable();
		for(i = 1; i < 9; i++)
			for(j = 1; j < 9; j++)
				drawChessman(i ,j, color[i][j]);
		if(colornow == BLA){	
			setfillstyle(1,WHITE);
			setcolor(LIGHTGRAY);
			fillellipse(STARTX + jnow * SIDELENGTH + SIDELENGTH / 2, STARTY + inow * SIDELENGTH + SIDELENGTH / 2, 2*RADIUS-k*2,2*RADIUS-k*2);
			delay(8000);
		}
		else if(colornow == WHI){
			setfillstyle(1,BLACK);
			setcolor(DARKGRAY);
			fillellipse(STARTX + jnow * SIDELENGTH + SIDELENGTH / 2, STARTY + inow * SIDELENGTH + SIDELENGTH / 2, 2*RADIUS-k*2,2*RADIUS-k*2);
			delay(8000);
		}
	}
}
		


/*
 * Name: void NewGame()
 * Parameter: None
 * Return: None
 * Function: Initialize the Reversi game
 */
void NewGame(){
	int i, j;
	setbkcolor(BLACK);
	drawTable();
	initializeTable();
	for(i = 1; i < 9; i++){
		for(j = 1; j < 9; j++){
			drawChessman(i,j,color[i][j]);
		}
	}
}

void select(){
	outtextxy(240,300,"Hum vs Hum");
	outtextxy(400,300,"Hum vs Cpu (Coming soon!)");
}