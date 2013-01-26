/*
 ============================================================================
 Name        : Reversi
 Author      : The_reAsOn
 Version     :
 Copyright   : The_reAsOn Copyright
 Description :
 ============================================================================
 */

#include "Reversi.h"

	
	int color[9][9];
	int mouseLeftFlag;
	int mx=10;
	int my=10;
	int mousex=10;
	int mousey=10;
	int colornow = BLA;
	int flag, bla, whi, count, stopcheck;
	int inow, jnow, ijnowcolor;
	char black[10];
	char white[10];
	void far *imagep;
	int win = 1;

int main(void) {
	unsigned long gsize;
	int a = 10, b = 10;
	InitializeGraph();
	/* 绘制鼠标图形 */
	setcolor(BLUE);
	line(a-MSIZE,b-MSIZE,a+MSIZE,b+MSIZE/2);
	line(a-MSIZE,b-MSIZE,a+MSIZE/2,b+MSIZE);
	line(a+MSIZE,b+MSIZE/2,a+MSIZE/2,b+MSIZE);
	/* 保存鼠标图形位图 */
	gsize=imagesize(a-MSIZE,b-MSIZE,a+MSIZE,b+MSIZE);
	imagep=malloc(gsize);
	getimage(a-MSIZE,b-MSIZE,a+MSIZE,b+MSIZE,imagep);
	cleardevice();
	do{
	do{	
		Welcome();
		MusicWithWelcome();
		select();
		if(leftPress() != 1){
			getMousePosition(&mx,&my);
			if(mousex != mx||mousey != my){
				putimage(mousex,mousey,imagep,XOR_PUT);
				mousex = mx; mousey = my;
				putimage(mousex,mousey,imagep,XOR_PUT);
			}
		}
		else if(mouseLeftFlag ==1){
			mouseLeftFlag = 0;
			getMousePosition(&mx,&my);
			if(mx < 280 && mx > 200 && my < 320 && my > 280){
				cleardevice();
				NewGame();
				Gaming();
				break;
			}
		}
	}while(1);
	}while(SureExit());
	return 0;
}
