/*
D: the_rea1
LANG: C
TASK: friday
*/
#include <stdio.h>
main () {

	int isleap(int year);
	void lyear_count(int start, int *week_days);
	void cyear_count(int start, int *week_days);
    FILE *fin  = fopen ("friday.in", "r");
    FILE *fout = fopen ("friday.out", "w");
	int year, start, n, n_count, day;
	static int week_days[6];

	year = 1900;
	start = 2;
	fscanf(fin, "%d", &n);
	for(n_count = 0; n_count < n; n_count++){
		if(isleap(year)==1){
			lyear_count(start, week_days);
			start = (start + 366) % 7;
		}else{
			cyear_count(start, week_days);
			start = (start + 365) % 7;
		}
		year++;
	}
	for(day = 0; day < 6; day++)
		fprintf(fout, "%d ", week_days[day]);
	fprintf(fout, "%d", week_days[day]);
	fprintf(fout, "\n");
	fclose(fin);
	fclose(fout);
    exit (0);
}

int isleap(int year){
	if((year % 100 != 0 && year % 4 == 0)||(year % 400 == 0))
		return 1;
	else
		return 0;
}


void lyear_count(int start, int *week_days){
	int ly_month[13] = {0,31,29,31,30,31,30,31,31,30,31,30,31};
	int now = start;	
	int month;
	for(month = 0; month < 12; month++){
		week_days[(start + 12) % 7]++;
		start = start + ly_month[month+1];
	}
}

void cyear_count(int start, int *week_days){
	int cy_month[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	int now = start;	
	int month;
	for(month = 0; month < 12; month++){
		week_days[(start + 12) % 7]++;
		start = start + cy_month[month+1];
	}
}
