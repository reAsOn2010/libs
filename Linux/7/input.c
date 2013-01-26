/*
	*input.c
	*created on 22 May 2012
	*author civi_3100101226
*/

/*the definition of the declaration*/

#include<stdio.h>
#include"input.h"
//print out the argument s and input an integer
double input(char *s)
{
	float x;
	printf("%s",s);
	scanf("%f",&x);
	return(x);
}
