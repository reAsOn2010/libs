/*
	*main.c
	*created on 22 May 2012
	*author civi_3100101226
*/

#include <stdio.h>
#include "main.h"
#include "compute.h"
#include "input.h"

/*the main part of this program*/
main()
{
	double x,y;
	printf("本程序从标准输入中获取x和y的值并显示x的y次方.\n");
	x = input(PROMPT1);
	y = input(PROMPT2);
	printf("x的y次方是:%6.3f\n",compute(x,y));
}
