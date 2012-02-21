/*
D: the_rea1
LANG: C
TASK: ride
*/
#include <stdio.h>
main () {
    FILE *fin  = fopen ("ride.in", "r");
    FILE *fout = fopen ("ride.out", "w");
    char ch1, ch2;
	int num1, num2;
	num1 = 1;
	num2 = 1;
    while(fscanf (fin, "%c", &ch1)!=EOF)
		if(ch1 >= 'A' && ch1 <= 'Z')
			num1 *= ch1 - 'A' + 1;
		else
			break;
    while(fscanf (fin, "%c", &ch2)!=EOF)
		if(ch2 >= 'A' && ch2 <= 'Z')
			num2 *= ch2 - 'A' + 1;
		else
			break;
	if((num1 % 47) == (num2 % 47))
    	fprintf (fout, "GO\n");
	else
    	fprintf (fout, "STAY\n");
	fclose(fin);
	fclose(fout);
    exit (0);
}
