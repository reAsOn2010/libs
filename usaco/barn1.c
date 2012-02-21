/*
D: the_rea1
LANG: C
TASK: barn1
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#define MAXSTALLS 201
int main () {
    FILE *fin  = fopen ("barn1.in", "r");
    FILE *fout = fopen ("barn1.out", "w");
	int nboard;
	int nstall;
	int ncows;
	int cows[MAXSTALLS] = {0};
	int i, j, temp;
	int min = 300, max = 0;
	int gaps[MAXSTALLS] = {0}, gap, maxgap = 0, maxindex = 0;
	int res;
	assert((fin != NULL) && (fout != NULL));
	fscanf(fin, "%d%d%d", &nboard, &nstall, &ncows);
	for(i = 0; i < ncows; i++){
		fscanf(fin, "%d", &temp);
		cows[temp] = 1;
		if(min > temp)
			min = temp;
		if(max < temp)
			max = temp;
	}
	gap = 0;
	for(i = min; i <= max; i++){
		if(!cows[i] && !cows[i + 1])
			gaps[gap]++;
		else if(!cows[i] && cows[i + 1]){
			gaps[gap]++;
			gap++;
		}
	}
	res = max - min + 1;
	for(i = 0; i < nboard - 1; i++){
		maxgap = maxindex = 0;
		for(j = 0; j < gap; j++){
			if(maxgap < gaps[j]){
				maxgap = gaps[j];
				maxindex = j;
			}
		}
		gaps[maxindex] = 0;
		res -= maxgap;
	}
	fprintf(fout, "%d\n", res);	
	fclose(fin);
	fclose(fout);
    exit (0);
}
