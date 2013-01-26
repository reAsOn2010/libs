/*
	* 	a easy program to generate Fibonacci sequence using 
		multi pthread
	*  	created on 1 june
	* 	author : civi 3100101226
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int a[100];

void *Fibonacci(void* n){
	int i;
	a[0] = 0;
	a[1] = 1;
	for (i=2;i<*((int*)n);i++){
		a[i] = a[i-1] + a[i-2];	
	}
}

int main(int argc, char *argv[])
{
	int i,n;
	pthread_t tid1;
	n = atoi(argv[1]);
	pthread_create(&tid1,NULL,Fibonacci,(void *)(&n));
	pthread_join(tid1,NULL);
	for (i=0;i<n;i++)
		printf("%d  ", a[i]);
	printf("\n");
	return 0;
}
