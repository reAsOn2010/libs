/*
D: the_rea1
LANG: C
TASK: transform
*/
#include <stdio.h>
#include <assert.h>
main () {
	void ro_90(char p[10][10], int n, char change[10][10]);
	void ro_180(char p[10][10], int n, char change[10][10]);
	void ro_270(char p[10][10], int n, char change[10][10]);
	void reflect(char p[10][10], int n, char change[10][10]);
	void com1(char p[10][10], int n, char change[10][10]);
	void com2(char p[10][10], int n, char change[10][10]);
	void com3(char p[10][10], int n, char change[10][10]);
	int compare(char a[10][10], char b[10][10], int n);

    FILE *fin  = fopen ("transform.in", "r");
    FILE *fout = fopen ("transform.out","w");
	int n, i, j, res;
	char temp;
	char pattern[10][10];
	char after[10][10];
	char change[10][10];

	assert((fin != NULL) && (fout != NULL));
	fscanf(fin, "%d", &n);
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			while(fscanf(fin, "%c", &temp))
				if(temp == '-' || temp == '@'){
					pattern[i][j] = temp;
					break;
				}
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			while(fscanf(fin, "%c", &temp))
				if(temp == '-' || temp == '@'){
					after[i][j] = temp;
					break;
				}
	for(res = 1; res < 7; res++){
		if(res == 1){
			ro_90(pattern, n , change);
			if(compare(after, change, n))
				break;
		}
		else if(res == 2){
			ro_180(pattern, n , change);
			if(compare(after, change, n))
				break;
		}
		else if(res == 3){
			ro_270(pattern, n , change);
			if(compare(after, change, n))
				break;
		}
		else if(res == 4){
			reflect(pattern, n , change);
			if(compare(after, change, n))
				break;
		}
		else if(res == 5){
			com1(pattern, n , change);
			if(compare(after, change, n))
				break;
			com2(pattern, n , change);
			if(compare(after, change, n))
				break;
			com3(pattern, n , change);
			if(compare(after, change, n))
				break;
		}
		else if(res == 6){
			if(compare(after, pattern, n))
				break;
		}
	}
	fprintf(fout, "%d\n", res);

	fclose(fin);
	fclose(fout);
    exit (0);
}

void ro_90(char p[10][10], int n, char change[10][10]){
	int i, j;
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			change[i][j] = p[n-j-1][i];
}

void ro_180(char p[10][10], int n, char change[10][10]){
	char temp[10][10];
	ro_90(p, n, temp);
	ro_90(temp, n, change);
}

void ro_270(char p[10][10], int n, char change[10][10]){
	char temp[10][10];
	ro_180(p, n, temp);
	ro_90(temp, n, change);
}

void reflect(char p[10][10], int n, char change[10][10]){
	int i, j;
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			change[i][j] = p[i][n-j-1];
}

void com1(char p[10][10], int n, char change[10][10]){
	char temp[10][10];
	reflect(p, n, temp);
	ro_90(temp, n, change);
}

void com2(char p[10][10], int n, char change[10][10]){
	char temp[10][10];
	reflect(p, n, temp);
	ro_180(temp, n, change);
}

void com3(char p[10][10], int n, char change[10][10]){
	char temp[10][10];
	reflect(p, n, temp);
	ro_270(temp, n, change);
}

int compare(char a[10][10], char b[10][10], int n){
	int i, j, res;
	res = 1;
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			if(a[i][j] != b[i][j]){
				res = 0;
				return res;
			}
	return res;
}
