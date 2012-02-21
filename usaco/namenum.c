/*
D: the_rea1
LANG: C
TASK: namenum
*/
#include <stdio.h>
#include <assert.h>
#include <string.h>
int main () {
	int w2t(char word[15], char temp[15]);
    FILE *fin  = fopen ("namenum.in", "r");
    FILE *fout = fopen ("namenum.out", "w");
	FILE *dict = fopen ("dict.txt", "r");
	
	char num[15];
	char word[15];
	char temp[15];
	int n = 0, i;

	assert((fin != NULL) && (fout != NULL) && (dict != NULL));
	for(i = 0; i < 15; i ++){
		num[i]='\0';
		word[i]='\0';
		temp[i]='\0';
	}
	fgets(num, 15, fin);
	do{
		for(i = 0; i < 15; i ++){
			word[i]='\0';
			temp[i]='\0';
		}
		fgets(word, 15, dict);
		w2t(word, temp);
		if(strcmp(num, temp)==0){
			fputs(word, fout);
			n++;
		}
	}while(feof(dict) == 0);
	if(n == 0)
		fprintf(fout, "NONE\n");
	fclose(dict);
	fclose(fin);
	fclose(fout);
    exit (0);
}

int w2t(char word[15], char temp[15]){
	int i;
	for(i = 0; i < 15; i++){
		switch(word[i]){
			case 'A':
			case 'B':
			case 'C':temp[i]='2';break;
			case 'D':
			case 'E':
			case 'F':temp[i]='3';break;
			case 'G':
			case 'H':
			case 'I':temp[i]='4';break;
			case 'J':
			case 'K':
			case 'L':temp[i]='5';break;
			case 'M':
			case 'N':
			case 'O':temp[i]='6';break;
			case 'P':
			case 'R':
			case 'S':temp[i]='7';break;
			case 'T':
			case 'U':
			case 'V':temp[i]='8';break;
			case 'W':
			case 'X':
			case 'Y':temp[i]='9';break;
			case 'Q':
			case 'Z':temp[i]='1';break;
			default:temp[i] = word[i];break;
		}
	}
	return 0;
}
		
