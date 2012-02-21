/*
D: the_rea1
LANG: C
TASK: milk
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#define MAXPRICE 1001
int main () {
    FILE *fin  = fopen ("milk.in", "r");
    FILE *fout = fopen ("milk.out", "w");
	int need, nfarmer;
	int price, milk;
	int total;
	int i;
	int amount[MAXPRICE] = {0};

	assert((fin != NULL) && (fout != NULL));
	fscanf(fin, "%d%d", &need, &nfarmer);
	for(i = 0; i < nfarmer; i++){
		fscanf(fin,"%d", &price);
		fscanf(fin,"%d", &milk);
		amount[price] += milk;
	}
	total = 0;
	for(i = 0; need > amount[i]; i++){
		total += i * amount[i];
		need -= amount[i];
	}
	total += need * i;
	fprintf(fout, "%d\n", total);
	fclose(fin);
	fclose(fout);
    exit (0);
}
