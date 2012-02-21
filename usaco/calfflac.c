/*
D: the_rea1
LANG: C
TASK: calfflac
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
int main () {
    FILE *fin  = fopen ("test.in", "r");
    FILE *fout = fopen ("test.out", "w");
	int input[20000], index = 0;
	char getc;
	int i;
	assert((fin != NULL) && (fout != NULL));
	while(feof(fin) != 0){
		fscanf(fin, "%c", getc);
		if(getc >= 'a' && getc <= 'z')
			input[index++] = getc;
		else if(getc >= 'A' && getc <= 'Z')
			input[index++] = getc - 'A' + 'a';
	}
	for(i = 1; i < index - 1; i++){

	}
	fclose(fin);
	fclose(fout);
    exit (0);
}
