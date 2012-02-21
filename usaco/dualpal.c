/*
D: the_rea1
LANG: C
TASK: dualpal
*/
#include <stdio.h>
#include <assert.h>
int main (){
    FILE *fin  = fopen ("dualpal.in", "r");
    FILE *fout = fopen ("dualpal.out", "w");
	int n, s, find, num;

    assert((fin != NULL) && (fout != NULL));
	find = 0;
	fscanf(fin, "%d%d", &n, &s);
	for(num = s + 1;find < n;num++){
		if(check(num)){
			fprintf(fout, "%d\n", num);
			find++;
		}
	}
	fclose(fin);
	fclose(fout);
	exit (0);
}

int check(num){
	int base;
	int temp;
	int after_ch[100];
	int i, j = 0;
	for(base = 2; base < 11; base++){
		temp = num;
		i = 0;
		while(temp > 0){
			after_ch[i++] = temp % base;
			temp = temp / base;
		}
		if(pal(after_ch, i))
			j++;
		if(j == 2)
			return 1;
	}
	return 0;
}

int pal(int num[], int i){
	int j;
	int res = 1;
	for(j = 0; j < i / 2; j++)
		if(num[j] != num[i - j - 1]){
			res = 0;
		}
	return res;
}
