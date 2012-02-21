#include<stdio.h>
#include<stdlib.h>
#include"tree.h"
#include<time.h>



FILE* creatTestCase(FILE *fp,int k);
clock_t start,finish;
int main()
{
	/*functions declarations*/
	int NF(double number[], int n, double *time);/*Next Fit Algorithm*/
	int FF(double number[], int n, double *time);/*First Fit Algorithm*/
	int BF(double number[], int n, double *time);/*Best Fit Algorithm*/
	int FFD(double number[], int n, double *time);/*First Fit Decreasing Algorithm*/
	
	
	
	/*We read txt files for input*/
	char path[100];/*to store the path of the input file*/
	FILE *fp,*fp1;/*file pointer*/
	double number[10000];/*the maximum number for reading*/
	double time = 0;/*calculate the time consuming for each Algorithm*/
	int n, ans;/*n is the total number of a case*//*ans give the number of bins to use*/
	int i = 0, j = 0;
	int CaseSize;
	
	char selection=-1;/*to choose the algorithm*/
	printf("Input the test case size(End with 0):\n");
	scanf("%d",&CaseSize);
	while(CaseSize)
	{
		fp1=creatTestCase(fp1,CaseSize);
		printf("Input the input file path(Done!):\n");/*the instructions to run the program*/
		printf("\tThe input file format:\n");/*give the input format*/
		printf("\tFirst line: N (how many bins)\n");
		printf("\tFollowing N lines: All the lines contain a number lays in (0.1)\n");
		/*scanf("%s",path);*//*read the path of the input file*/
		fp = fopen( "test.txt", "r");
		fscanf(fp, "%d", &n);
		for(j = 0; j < n; j++)/*read the input file for the test case*/
		{
			fscanf(fp, "%lf", &number[i++]);
		}
		printf("Input the parameter:\n");/*the insturctions to use different algorithms*/

		printf("\t0: Exit Test\n");
		printf("\t1: Next Fit\n");
		printf("\t2: First Fit\n");
		printf("\t3: Best Fit\n");
		printf("\t4: First Fit Decreasing\n");
	
		scanf("%c", &selection);/*avoid the '\n' disturbing read the choice*/
		scanf("%c", &selection);
		while(selection!='0')
		{
				switch(selection)/*choose the different algorithm*/
				{	
					case '1': ans = NF(number, n, &time);break;
					case '2': ans = FF(number, n, &time);break;
					case '3': ans = BF(number, n, &time);break;
					case '4': ans = FFD(number, n, &time);break;
					default: ans = -1;/*if the out put is -1, nothing is done*/
				}
				printf("ans = %d\n", ans);/*give the answer of the chosen algorithm*/
				printf("time consuming = %lf\n", time);/*give the time to run the function*/
			
			
				printf("Input the parameter:\n");/*the insturctions to use different algorithms*/
			/*
				printf("\t0: Exit Test\n");
				printf("\t1: Next Fit\n");
				printf("\t2: First Fit\n");
				printf("\t3: Best Fit\n");
				printf("\t4: First Fit Decreasing\n");
			*/
				scanf("%c", &selection);/*avoid the '\n' disturbing read the choice*/
				scanf("%c", &selection);
			/*the out put of number[], to test the qsort function*/
			/*
			for(j = 0; j < n; j++)
				printf("%lf ", number[j]);
			printf("\n");
			*/
		}
	printf("Input the test case size(End with 0):\n");
	scanf("%d",&CaseSize);
	}
	return 0;
}

int NF(double number[], int n, double *time)/*Next Fit Algorithm*/
{
	int i;
	int j;
	double next_bin = 0.0;/*Next Fit Algorithm will only the remember the former bin*/
	int bin = 1;

	if(n == 0)/*If nothing to put into the bin, return 0*/
		return 0;
	/*--Insert time functions--*/
for(j=0;j<1000;j++)
{
	start=clock();
	for(i = 0; i < n; i++)/*every new element inputs, operate once*/
	{
		if(next_bin + number[i] <= 1)/*if the former bin can contain the new one*/
			next_bin += number[i];/*put the new one into this bin*/
		else/*if cannot*/
		{
			bin++;/*create a new bin to store the new one*/
			next_bin = number[i];/*the new bin is used to store the new element*/
		}
	}
	/*--Insert time functions--*/
	finish=clock();
	*time+=finish-start;
}
	*time=(double)(*time)/CLOCKS_PER_SEC;
	return bin;/*return the answer*/
}

int FF(double number[], int n, double *time)/*First Fit Algorithm*/
{
	int i, j;
	double bins[10000] = {0.0};/*First Fit needs to know all the former bins' situation*/
	int bin = 0;/*this bin is storing the top index of bins[]*/

	if(n == 0)/*If nothing to put into the bin, return 0*/
		return 0;

	/*--Insert time functions--*/
	start=clock();
	for(i = 0; i < n; i++)/*every new element inputs, operate once*/
	{
		for(j = 0; j <= bin; j++)/*to find the first bin that can store the new element*/
		{
			if(bins[j] + number[i] <= 1)/*if the new element can be put in*/
			{
				bins[j] += number[i];/*update this bin*/
				break;/*break the for loop*/
			}
		}
		if(j > bin)/*if j is larger than bin, means no former bin can store the new element*/
		{
			bin++;/*so create a new bin*/
			bins[bin] = number[i];/*the new bin is used to store the new element*/
		}
	}
	/*--Insert time functions--*/
	finish=clock();
	*time=(double)(finish-start)/CLOCKS_PER_SEC;
	return bin + 1;/* the index plus 1 is the total used bins*/
}

int BF(double number[], int n, double *time)/*Best Fit Algorithm*/
{

	double temp_remain;/*used to store the target bin's remain capacity*/
	int i;
	int bin = 1;
	search_tree t = NULL;
	position temp;/*used in the search tree*/
	position parent;/*if cannot found the exact bin, this will store the nearest bin*/ 

	if(n == 0)/*If nothing to put into the bin, return 0*/
		return 0;
	
	t = insert(1.0, t);/*create the search tree*/
	/*--Insert time functions--*/
	start=clock();
	for(i = 0; i < n; i++)
	{
		temp = t;/*Initialize the positions*/
		parent = NULL;
		while(temp != NULL)/*haven't reach the botton*/
		{
			if(number[i] < temp->element)/*search the bin fit best*/
			{
				parent = temp;/*record the bin have bigger remain capacity*/
				temp = temp->left;/*go left*/
			}
			else if(number[i] > temp->element)
			{
				temp = temp->right;/*go right*/
			}
			else/*find the equal one*/
			{
				parent = temp;
				break;
			}
		}
		if(parent == NULL)/*cannot find the fit bin*/
		{
			bin++;/*create a new bin*/
			t = insert(1.0-number[i], t);/*insert the bin*/
		}
		else
		{
			temp_remain = retrieve(parent);/*get the remain capacity*/
			t = del(temp_remain, t);/*delete that node from the tree*/
			if(temp_remain - number[i] != 0)/*if there is still some remain capacity*/
				t = insert(temp_remain - number[i], t);/*insert back to the tree*/
		}
	}
	/*--Insert time functions--*/
	finish=clock();
	*time=(double)(finish-start)/CLOCKS_PER_SEC;
	return bin;
}


int compare(const void *a, const void *b)/*compare function used in BF*/
{
	return (*((double*)a) > *((double*)b));/*this decided the qsort former is sort the elements in increasing order*/
}

int FFD(double number[], int n, double *time)/*First Fit Decreasing Algorithm*/
{
	/*It's an off-line algorithm, sort the elements before applying  FF Algorithm*/
	int ans;
	int compare_d(const void *a, const void *b);
	int FF(double number[], int n, double *time);

	/*--Insert time functions--*/
	start=clock();
	qsort(number, (size_t)n, (size_t)(sizeof(double)), compare_d);/*sort the elements in decreasing order*/
	ans = FF(number, n, time);/*then apply the First Fit Algorithm*/
	/*--Insert time functions--*/
	finish=clock();
	*time=(double)(finish-start)/CLOCKS_PER_SEC;
	return ans;
}

int compare_d(const void *a, const void *b)
{
	return (*((double*)a) < *((double*)b));/*this decided the qsort former is sort the elements in decreasing order*/
}
FILE* creatTestCase(FILE *fp,int k)
{

int j;

fp=fopen("test.txt","w");
fprintf(fp, "%d\n", k);
for(j = 0; j < k; j++)
	{
		fprintf(fp, "%lf\n",0.1*(rand()%10));
	}
return fp;
}
