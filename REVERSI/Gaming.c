/*
 * Gaming.c
 *
 *  Created on: 2011-5-8
 *      Author: The_reAsOn
 */

#include "Reversi.h"

int Gaming(){
	int i, j, k, flag;
	stopcheck = 0;
	bla = whi = 2;
	printScore();
	while(CheckWin()){
		if(leftPress() != 1){
			getMousePosition(&mx,&my);
			if(mousex != mx||mousey != my){
				putimage(mousex,mousey,imagep,XOR_PUT);
				mousex = mx; mousey = my;
				putimage(mousex,mousey,imagep,XOR_PUT);
			}
		}
		else if(mouseLeftFlag == 1)			/*得到点击的位置*/
		{
			mouseLeftFlag = 0;
			getMousePosition(&mx,&my);
			for(i = 1; i < 9; i++){
				for(j = 1; j < 9; j++){
					if((mx > STARTX + SIDELENGTH * j && mx < STARTX + SIDELENGTH * (j+1)) && (my > STARTY + SIDELENGTH * i && my < STARTY + SIDELENGTH * (i+1))){
						break;
					}
				}
				if((mx > STARTX + SIDELENGTH * j && mx < STARTX + SIDELENGTH * (j+1)) && (my > STARTY + SIDELENGTH * i && my < STARTY + SIDELENGTH * (i+1))){
						break;
				}
			}
			if(color[i][j] == EMPTY){
				color[i][j] = colornow;
				inow = i; jnow = j; 
				if(change(i,j) == 1){
					if(BLA == colornow){
						ChessBlaSound();
						colornow = WHI;
					}
					else{
						ChessWhiSound();
						colornow = BLA;
					}
				}
				else{
					WarningSound();
					color[i][j] = EMPTY;
				}
			}
			cleardevice();
			drawTable();
			flag = bla = whi = count = 0;
			for(i = 1; i < 9; i++){
				for(j = 1; j < 9; j++){
					if(color[i][j] == BLA){
						bla++;
						count++;
					}else if(color[i][j] == WHI){
						whi++;
						count++;
					}else if(color[i][j] == EMPTY){
						color[i][j] = colornow;
						if(determine(i,j) == 1)
							flag = 1;
						color[i][j] = EMPTY;
					}
					drawChessman(i,j,color[i][j]);
				}
			}
			if(color[inow][jnow] != EMPTY)
				drawChessnow();
			if(flag == 0){
				if(BLA == colornow)
					colornow = WHI;
				else
					colornow = BLA;
				for(i = 1; i < 9; i++){
					for(j = 1; j < 9; j++){
						if(color[i][j] == EMPTY){
							color[i][j] = colornow;
							if(determine(i,j) == 1)
								flag = 1;
							color[i][j] = EMPTY;
						}
					}
				}
				if(flag == 0)
					stopcheck = 1;
			}
			printScore();
		}
	}
	return 1;
}


int CheckWin(){
	if(bla != 0 && whi != 0 && count < 64){
		if(stopcheck == 0)
			return 1;
		else if(stopcheck == 1){
			if(bla > whi){
				cleardevice();
				outtextxy(320,210,"Black Win!");
				outtextxy(320,280,"Press any key to continue");
				return 0;
			}
			else if(bla < whi){
				cleardevice();
				outtextxy(320,210,"White Win!");
				outtextxy(320,280,"Press any key to continue");
				return 0;
			}
			else{
				cleardevice();
				outtextxy(320,210,"Tie to Tie!");
				outtextxy(320,280,"Press any key to continue");
				return 0;
			}
		}
	}
	else if(bla == 0 && whi != 0){
		cleardevice();
		outtextxy(320,210,"White Win!");
		outtextxy(320,280,"Press any key to continue");
		return 0;
	}else if(whi == 0 && bla != 0){
		cleardevice();
		outtextxy(320,210,"Black Win!");
		outtextxy(320,280,"Press any key to continue");
		return 0;
	}else if(count == 64){
		if(bla > whi){		
			cleardevice();
			outtextxy(320,210,"Black Win!");
			outtextxy(320,280,"Press any key to continue");
			return 0;
		}else if(bla < whi){
			cleardevice();
			outtextxy(320,210,"White Win!");
			outtextxy(320,280,"Press any key to continue");
			return 0;
		}else{
			cleardevice();
			outtextxy(320,210,"Tie to Tie!");
			outtextxy(320,280,"Press any key to continue");
			return 0;
		}
	}
}

int SureExit(){
		outtextxy(320,100,"Play Again?");
		outtextxy(240,300,"Yes");
		outtextxy(400,300,"No and Quit");
	do{
		if(leftPress() != 1){
			getMousePosition(&mx,&my);
			if(mousex != mx||mousey != my){
				putimage(mousex,mousey,imagep,XOR_PUT);
				mousex = mx; mousey = my;
				putimage(mousex,mousey,imagep,XOR_PUT);
			}
		}else if(mouseLeftFlag == 1){
			mouseLeftFlag = 0;
			getMousePosition(&mx,&my);
			if(mx < 280 && mx > 200 && my < 320 && my > 280){
				cleardevice();
				return 1;
			}
			else if(mx < 440 && mx  > 360 && my < 320 && my > 280){
				cleardevice();
				return 0;
			}
		}
	}while(1);
}

void printScore(){

	if(colornow == BLA){
		setcolor(RED);
		setfillstyle(SOLID_FILL,RED);
		bar(20,66,26,72);
	}else if(colornow == WHI){
		setcolor(RED);
		setfillstyle(SOLID_FILL,RED);
		bar(420,66,426,72);
	}
		setcolor(WHITE);

		sprintf(black,"Score now: %d",bla);
		outtextxy(100,70,"Player 1 (Black)");
		outtextxy(100,80,black);
		sprintf(white,"Score now: %d",whi);
		outtextxy(500,70,"Player 2 (White)");
		outtextxy(500,80,white);
}