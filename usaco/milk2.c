/*
D: the_rea1
LANG: C
TASK: milk2
*/
#include <stdio.h>
#include <assert.h>
main () {
    FILE *fin  = fopen ("milk2.in", "r");
	FILE *fout = fopen ("milk2.out", "w");
	static short time[1000001];
	int n, i, j;
	int continuous = 0, idle = 0, t;
	int max_cont = 0, max_idle = 0;
	int begin = 1000001, end = 0, begin_now, end_now;
	assert((fin != NULL) && (fout != NULL));
	fscanf(fin ,"%d", &n);
	for(i = 0; i < n; i++){
		fscanf(fin, "%d%d", &begin_now, &end_now);
		for(j = begin_now + 1; j <= end_now; j++){
			time[j] = 1;
		}
		if(begin > begin_now)
			begin = begin_now;
		if(end < end_now)
			end = end_now;
	}
	for(t = begin + 1; t <= end; t++){
		if(time[t] == 1){
			continuous++;
			idle = 0;
			if(continuous > max_cont)
				max_cont = continuous;
		}
		else if(time[t] == 0){
			idle++;
			continuous = 0;
			if(idle > max_idle)
				max_idle = idle;
		}
	}
	fprintf(fout, "%d %d\n", max_cont, max_idle);
	fclose(fin);
	fclose(fout);
    exit (0);
}
