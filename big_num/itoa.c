#include "stdio.h"

char *itoa(long i);
int main(int argc,char *argv[]){
  long i;
  char *a;
  if (argc!=2)
    return -1;
  i=atol(argv[1]);
  a=itoa(i);
  printf("%s\n",a);
  return 0;
}
char *itoa(long i){
  char *answer;
  long j=10;
  long k=0;
  while(i%j!=i)j*=10;
  j/=10;
  while(j!=0){
    answer[k++]=i/j+'0';
    i%=j;
    j/=10;
  }
  return answer;
}
