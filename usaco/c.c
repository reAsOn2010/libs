/*
D: the_rea1
LANG: C
TASK: test
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
int main () {
    FILE *fin  = fopen ("test.in", "r");
    FILE *fout = fopen ("test.out", "w");
	assert((fin != NULL) && (fout != NULL));
	fclose(fin);
	fclose(fout);
    exit (0);
}
