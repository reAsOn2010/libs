/*
 * Change.c
 *
 *  Created on: 2011-5-15
 *      Author: dell
 */
#include "Reversi.h"


int change(int x, int y) 
{
	int right(int x,int y);
    int left(int x,int y);
    int up(int x,int y);
    int down(int x,int y);
    int leftup(int x,int y);
    int rightup(int x,int y);
    int rightdown(int x,int y);
    int leftdown(int x,int y);
    int left_,up_,right_,down_,leftup_,rightup_,rightdown_,leftdown_;
    
	left_ = left(x,y);
	up_ = up(x,y);
	right_ = right(x,y);
	down_ = down(x,y);
	leftup_ = leftup(x,y);
	rightup_ = rightup(x,y);
	rightdown_ = rightdown(x,y);
	leftdown_ = leftdown(x,y);

	if(left_||up_||right_||down_||leftup_||rightup_||rightdown_||leftdown_ == 1)
		return 1;
	else 
		return 0;
}

int up(int x,int y)
{
	if(x > 0 && x < 9 && y > 0 && y < 9 && x > 1){
	int xi,xx,flag;
	flag = 0;
	for(xi = x-1; xi > 0 && xi < 9; xi--){
		if(color[xi][y]==0||color[xi][y]==color[x][y]){
			break;
		}
	}
	if(color[xi][y]!=0 && xi > 0){
		for(xx = x-1; xx >= xi && xx > 0 && xx < 9; xx--){
			if(color[xx][y]!=color[x][y]&&color[xx][y]!=0){
				color[xx][y] = color[x][y];
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int left(int x,int y)
{
	if(x > 0 && x < 9 && y > 0 && y < 9 && y > 1){
	int yi,yy,flag;
	flag = 0;
	for(yi = y-1; yi > 0 && yi < 9; yi--){
		if(color[x][yi]==0||color[x][yi]==color[x][y]){
			break;
		}
	}
	if(color[x][yi]!=0 && yi > 0){
		for(yy = y-1; yy >= yi && yy > 0 && yy < 9; yy--){
			if(color[x][yy]!=color[x][y]&&color[x][yy]!=0){
				color[x][yy] = color[x][y];
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int down(int x,int y)
{
	if(x > 0 && x < 9 && y > 0 && y < 9 && x < 7){
	int xi,xx,flag;
	flag = 0;
	for(xi = x+1; xi > 0 && xi < 9; xi++){
		if(color[xi][y]==0||color[xi][y]==color[x][y]){
			break;
		}
	}
	if(color[xi][y]!=0 && xi < 9){
		for(xx = x+1; xx <= xi && xx > 0 && xx < 9; xx++){
			if(color[xx][y]!=color[x][y]&&color[xx][y]!=0){
				color[xx][y] = color[x][y];
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int right(int x,int y)
{
	if(x > 0 && x < 9 && y > 0 && y < 9 && y < 7){
	int yi,yy,flag;
	flag = 0;
	for(yi = y+1; yi > 0 && yi < 9; yi++){
		if(color[x][yi]==0||color[x][yi]==color[x][y]){
			break;
		}
	}
	if(color[x][yi]!=0 && yi < 9){
		for(yy = y+1; yy <= yi && yy > 0 && yy < 9; yy++){
			if(color[x][yy]!=color[x][y]&&color[x][yy]!=0){
				color[x][yy] = color[x][y];
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int leftup(int x,int y)
{
	if(x > 0 && x < 9 && y > 0 && y < 9 && x > 1 && y > 1){
	int xi,xx,yi,yy,flag;
	flag = 0;
	for(xi = x-1,yi = y-1; xi > 0 && yi > 0 && xi < 9 && yi < 9; xi--,yi--){
		if(color[xi][yi]==0||color[xi][yi]==color[x][y]){
			break;
		}
	}
	if(color[xi][yi]!=0 && xi > 0 && yi > 0){
		for(xx = x-1,yy = y-1; xx >= xi && xx > 0 && yy > 0 && xx < 9 && yy < 9;xx--,yy--){
			if(color[xx][yy]!=color[x][y]&&color[xx][yy]!=0){
				color[xx][yy] = color[x][y];
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int leftdown(int x,int y)
{
	if(x > 0 && x < 9 && y > 0 && y < 9 && x < 7 && y > 1){
	int xi,xx,yi,yy,flag;
	flag = 0;
	for(xi = x+1,yi = y-1; xi > 0 && yi > 0 && xi < 9 && yi < 9; xi++,yi--){
		if(color[xi][yi]==0||color[xi][yi]==color[x][y]){
			break;
		}
	}
	if(color[xi][yi]!=0 && xi < 9 && yi > 0){
		for(xx = x+1,yy = y-1; xx <= xi && xx > 0 && yy > 0 && xx < 9 && yy < 9;xx++,yy--){
			if(color[xx][yy]!=color[x][y]&&color[xx][yy]!=0){
				color[xx][yy] = color[x][y];
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int rightdown(int x,int y)
{
	if(x > 0 && x < 9 && y > 0 && y < 9 && x < 7 && y < 7){
	int xi,xx,yi,yy,flag;
	flag = 0;
	for(xi = x+1,yi = y+1; xi > 0 && yi > 0 && xi < 9 && yi < 9; xi++,yi++){
		if(color[xi][yi]==0||color[xi][yi]==color[x][y]){
			break;
		}
	}
	if(color[xi][yi]!=0 && xi < 9 && yi < 9){
		for(xx = x+1,yy = y+1; xx <= xi && xx > 0 && yy > 0 && xx < 9 && yy < 9;xx++,yy++){
			if(color[xx][yy]!=color[x][y]&&color[xx][yy]!=0){
				color[xx][yy] = color[x][y];
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int rightup(int x,int y)
{
	if(x > 0 && x < 9 && y > 0 && y < 9 && x > 1 && y < 7){
	int xi,xx,yi,yy,flag;
	flag = 0;
	for(xi = x-1,yi = y+1; xi > 0 && yi > 0 && xi < 9 && yi < 9; xi--,yi++){
		if(color[xi][yi]==0||color[xi][yi]==color[x][y]){
			break;
		}
	}
	if(color[xi][yi]!=0 && xi > 0 && yi < 9){
		for(xx = x-1,yy = y+1; xx >= xi && xx > 0 && yy > 0 && xx < 9 && yy < 9;xx--,yy++){
			if(color[xx][yy]!=color[x][y]&&color[xx][yy]!=0){
				color[xx][yy] = color[x][y];
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}



int determine(int x, int y){
    int left_d,up_d,right_d,down_d,leftup_d,rightup_d,rightdown_d,leftdown_d;
	
	left_d = leftd(x,y);
	up_d = upd(x,y);
	right_d = rightd(x,y);
	down_d = downd(x,y);
	leftup_d = leftupd(x,y);
	rightup_d = rightupd(x,y);
	rightdown_d = rightdownd(x,y);
	leftdown_d = leftdownd(x,y);

	if(left_d||up_d||right_d||down_d||leftup_d||rightup_d||rightdown_d||leftdown_d == 1)
		return 1;
	else
		return 0;
}

int upd(int x,int y){
	int xi,xx,flag;
	if(x > 0 && x < 9 && y > 0 && y < 9 && x > 1){

	flag = 0;
	for(xi = x-1; xi > 0 && xi < 9; xi--){
		if(color[xi][y]==EMPTY||color[xi][y]==color[x][y]){
			break;
		}
	}
	if(color[xi][y]!=0 && xi > 0){
		for(xx = x-1; xx >= xi && xx > 0 && xx < 9; xx--){
			if(color[xx][y]!=color[x][y]&&color[xx][y]!=EMPTY){
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int leftd(int x,int y){
	int yi,yy,flag;
	if(x > 0 && x < 9 && y > 0 && y < 9 && y > 1){

	flag = 0;
	for(yi = y-1; yi > 0 && yi < 9; yi--){
		if(color[x][yi]==EMPTY||color[x][yi]==color[x][y]){
			break;
		}
	}
	if(color[x][yi]!=0 && yi > 0){
		for(yy = y-1; yy >= yi && yy > 0 && yy < 9; yy--){
			if(color[x][yy]!=color[x][y]&&color[x][yy]!=EMPTY){
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int downd(int x,int y){
	int xi,xx,flag;
	if(x > 0 && x < 9 && y > 0 && y < 9 && x < 7){

	flag = 0;
	for(xi = x+1; xi > 0 && xi < 9; xi++){
		if(color[xi][y]==EMPTY||color[xi][y]==color[x][y]){
			break;
		}
	}
	if(color[xi][y]!=0 && xi < 9){
		for(xx = x+1; xx <= xi && xx > 0 && xx < 9; xx++){
			if(color[xx][y]!=color[x][y]&&color[xx][y]!=EMPTY){
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int rightd(int x,int y){
	int yi,yy,flag;
	if(x > 0 && x < 9 && y > 0 && y < 9 && y < 7){

	flag = 0;
	for(yi = y+1; yi > 0 && yi < 9; yi++){
		if(color[x][yi]==EMPTY||color[x][yi]==color[x][y]){
			break;
		}
	}
	if(color[x][yi]!=0 && yi > 9){
		for(yy = y+1; yy <= yi && yy > 0 && yy < 9; yy++){
			if(color[x][yy]!=color[x][y]&&color[x][yy]!=EMPTY){
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int leftupd(int x,int y){
	int xi,xx,yi,yy,flag;
	if(x > 0 && x < 9 && y > 0 && y < 9 && x > 1 && y > 1){

	flag = 0;
	for(xi = x-1,yi = y-1; xi > 0 && yi > 0 && xi < 9 && yi < 9; xi--,yi--){
		if(color[xi][yi]==EMPTY||color[xi][yi]==color[x][y]){
			break;
		}
	}
	if(color[xi][yi]!=0 && xi > 0 && yi > 0){
		for(xx = x-1,yy = y-1; xx >= xi && xx > 0 && yy > 0 && xx < 9 && yy < 9;xx--,yy--){
			if(color[xx][yy]!=color[x][y]&&color[xx][yy]!=EMPTY){
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int leftdownd(int x,int y){
	int xi,xx,yi,yy,flag;
	if(x > 0 && x < 9 && y > 0 && y < 9 && x < 7 && y > 1){

	flag = 0;
	for(xi = x+1,yi = y-1; xi > 0 && yi > 0 && xi < 9 && yi < 9; xi++,yi--){
		if(color[xi][yi]==EMPTY||color[xi][yi]==color[x][y]){
			break;
		}
	}
	if(color[xi][yi]!=0 && xi < 9 && yi > 0){
		for(xx = x+1,yy = y-1; xx <= xi && xx > 0 && yy > 0 && xx < 9 && yy < 9;xx++,yy--){
			if(color[xx][yy]!=color[x][y]&&color[xx][yy]!=EMPTY){
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int rightdownd(int x,int y){
	int xi,xx,yi,yy,flag;
	if(x > 0 && x < 9 && y > 0 && y < 9 && x < 7 && y < 7){

	flag = 0;
	for(xi = x+1,yi = y+1; xi > 0 && yi > 0 && xi < 9 && yi < 9; xi++,yi++){
		if(color[xi][yi]==EMPTY||color[xi][yi]==color[x][y]){
			break;
		}
	}
	if(color[xi][yi]!=0 && xi < 9 && yi < 9){
		for(xx = x+1,yy = y+1; xx <= xi && xx > 0 && yy > 0 && xx < 9 && yy < 9;xx++,yy++){
			if(color[xx][yy]!=color[x][y]&&color[xx][yy]!=EMPTY){
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}

int rightupd(int x,int y){
	int xi,xx,yi,yy,flag;
	if(x > 0 && x < 9 && y > 0 && y < 9 && x > 1 && y < 7){

	flag = 0;
	for(xi = x-1,yi = y+1; xi > 0 && yi > 0 && xi < 9 && yi < 9; xi--,yi++){
		if(color[xi][yi]==EMPTY||color[xi][yi]==color[x][y]){
			break;
		}
	}
	if(color[xi][yi]!=0 && xi > 0 && yi < 9){
		for(xx = x-1,yy = y+1; xx >= xi && xx > 0 && yy > 0 && xx < 9 && yy < 9;xx--,yy++){
			if(color[xx][yy]!=color[x][y]&&color[xx][yy]!=EMPTY){
				flag = 1;
			}
		}
	}
	return flag;
	}
	else return 0;
}