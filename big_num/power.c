#include "./calculation.h"

char *exponentiation(char *base,long power){
  char *answer;
  char *remain="1";
  answer=base;
  while(power!=1){
    if(power%2)
      remain=multiple(remain,answer);
    answer=multiple(answer,answer);
    power/=2;
  }
  answer=multiple(answer,remain);
  return answer;
}
int main(int argc,char *argv[]){
  char *answer;
  if(argc!=3){
    printf("input 2 numbers\n");
    return -1;
  }
  answer=exponentiation(argv[1],atol(argv[2]));
  printf("%s\n",answer);
  return 0;
}
