/*It's the *.h file for the errorlogs*/
#include<stdio.h>
#include<stdlib.h>

void space_fatal_error();
void error_log(char* info);

void space_fatal_error(){/*fatal error of malloc failure*/
	printf("Out of space!!!\n");
	exit(1);/*fatal error leads to quit the program*/
}

void error_log(char* info)/*normal error and log the infomation*/
{
	printf("%s\n",info);
}
