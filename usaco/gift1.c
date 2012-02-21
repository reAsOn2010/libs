/*
D: the_rea1
LANG: C
TASK: gift1
*/
#include <stdio.h>
#include <string.h>
main () {
    FILE *fin  = fopen ("gift1.in", "r");
    FILE *fout = fopen ("gift1.out", "w");
	int np, ng, total;
	int i, j;
	char name[20][100];
	char temp[100];
	int money[20];
	for(i = 0; i < 20; i++)
		money[i] = 0;
	fscanf(fin, "%d", &np);
	for(i = 0; i < np; i++){
		fscanf(fin, "%s", name[i]);
	}
	while(fscanf(fin, "%s", temp)!=EOF){
		for(i = 0; i < np; i++)
			if(strcmp(name[i],temp)==0){
				fscanf(fin, "%d %d",&total, &ng);
				money[i] -= total;
				if(ng != 0)
					money[i] += total % ng;
			}
		for(j = 0; j < ng; j++){
			fscanf(fin, "%s", temp);
			for(i = 0; i < np; i++)
				if(strcmp(name[i],temp) == 0){
					money[i] += total/ng;
				}
		}
	}
	for(i = 0; i < np; i++){
		fprintf(fout,"%s %d\n",name[i],money[i]);
	}
	fclose(fin);
	fclose(fout);
	exit (0);
}
