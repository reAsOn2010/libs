 #include "stdio.h"  
#include "stdlib.h"  
#include "string.h"  

void plusplus(char *num,char *answer);  

int main(int argc,char *argv[]){  
char *answer;  
    if (argc!=2)  
      return -1;  
    answer=(char *)malloc((strlen(argv[1]+1)*sizeof(char)));  
    plusplus(argv[1],answer);  
    printf("%s\n",answer);  
    return 0;  
  }  

void plusplus(char *num,char *answer){
  char *temp;
  temp=(char *)malloc((strlen(num)+1)*sizeof(char));
  temp[strlen(num)]=0;
  temp[0]='0';temp[1]=0;
  strcat(temp,num);
  temp+=strlen(num);
  num=temp-strlen(num);
  while(*temp=='9'){
    *temp='0';
    temp--;
  }
  (*temp)++;
  if(*num=='0')
    strcpy(answer,(++num));
  else
    strcpy(answer,num);
  return;
}
