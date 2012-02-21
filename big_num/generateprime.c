#include "stdio.h"
#include "stdlib.h"
#include "string.h"
void generate(long num);

int main(int argc,char *argv[]){
  char **answer;
  if(argc!=2)
    return -1;
  generate(atol(argv[1]));
}

void generate(long num){
  short int *all_num;
  long j,i;
  long count=0;
  num/=2;
  all_num=(short int *)malloc(num*sizeof(short int));
  for (i=0;i<num;i++)
    all_num[i]=1;
  all_num[0]=0;
  i=0;
  do{
    while((!(all_num[++i]))&&(i<num));
    if(i>=num)
      break;
    count++;
    for(j=i*3+1;j<num;j+=(i*2+1))
      all_num[j]=0;
  }while(1);
  printf("2\n");
  for(i=0;i<num;i++)
    if(all_num[i])
      printf("%ld\n",i*2+1);
}
