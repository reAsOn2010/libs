/*
D: the_rea1
LANG: C
TASK: palsquare
*/
#include <stdio.h>
#include <assert.h>

int main () {
    FILE *fin  = fopen ("palsquare.in", "r");
    FILE *fout = fopen ("palsquare.out", "w");
	int base, bit_squ, bit_num, i;
	int num;
	int base_num[100], base_squ[100];
	char ch = 'A';
	assert((fin != NULL) && (fout != NULL));
	fscanf(fin, "%d", &base);
	for(num = 1; num < 301; num++){
		if(check(num, base_num, base_squ, base, &bit_num, &bit_squ)){
			for(i = 0; i < bit_num; i++){
				if(base_num[i] >= 10)
					fprintf(fout, "%c", ch + (base_num[i] - 10));
				else
					fprintf(fout, "%d", base_num[i]);
			}
			fprintf(fout, " ");
			for(i = 0; i < bit_squ; i++){
				if(base_squ[i] >= 10)
					fprintf(fout, "%c", ch + (base_squ[i] - 10));
				else
					fprintf(fout, "%d", base_squ[i]);
			}
			fprintf(fout, "\n");
		}
	}
	fclose(fin);
	fclose(fout);
    exit (0);
}

int check(int num, int base_num[], int base_squ[], int base, int *bit_num, int *bit_squ){
	void reverse(int base_num[], int i);
	int i = 0, res, j;
	int squ = num*num;
	while(num > 0){
		base_num[i++] = num % base;
		num = num / base;
	}
	reverse(base_num, i);
	*bit_num = i;
	i = 0;
	while(squ > 0){
		base_squ[i++] = squ % base;
		squ = squ / base;
	}
	reverse(base_squ, i);
	res = 1;
	for(j = 0; j < i / 2; j++)
		if(base_squ[j] != base_squ[i - j - 1])
				res = 0;
	*bit_squ = i;
	return res;
}

void reverse(int base_num[], int i){
	int j;
	int temp;
	for(j = 0; j < i / 2; j++){
		temp = base_num[j];
		base_num[j] = base_num[i - j - 1];
		base_num[i - j - 1] = temp;
	}
}
