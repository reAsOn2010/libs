  #include "stdio.h"  
  #include "string.h"  
  #include "stdlib.h"  

  char *plus(char *a,char *b);  
  int main(int argc,char *argv[]){  
    char *answer;  
    if (argc!=3){  
      printf("input 2 numbers\n");  
      return -1;  
    }  
    else  
      answer=plus(argv[1],argv[2]);  
    printf("The Answer is:%s\n",answer);  
    return 0;  
  }  

/*plus函数参数为两个字符串，表示参与加法所需的两个数
函数返回一个字符串地址，里面存储了字符串形式的所加值的地址
参与加法的数只允许是正数，负数可直接使用减法运算*/
char *plus(char *a,char *b){
  long len_a=0,len_b=0;
  char *p;
  short int addup=0/*进位*/,temp=0;
  long i=0,j=0;
  char *answer;
  len_a=strlen(a);
  len_b=strlen(b);
  /*确保len_a>len_b*/
  if(len_a<len_b){
    len_a=len_a^len_b;
    len_b=len_a^len_b;
    len_a=len_a^len_b;
    p=a;
    a=b;
    b=p;
  }
  answer=(char *)malloc(len_a*sizeof(char));
  for(i=len_b-1;i>=0;i--){
    temp=b[i]+a[i+len_a-len_b]-'0'-'0'+addup;
    addup=0;
    if(temp>9){
      temp-=10;
      addup=1;
    }
    answer[j++]=temp+'0';
  }
  i=len_a-len_b-1;
  while(i>=0){
    answer[j++]=a[i--]+addup;
    addup=0;
    if(answer[j-1]>'9'){
      answer[j-1]-=10;
      addup=1;
    }
  }
  if(addup)
    answer[j++]='1';
  answer[j]=0;
  for(i=strlen(answer)-1,j=0;j<(strlen(answer)/2);i--,j++){
    answer[i]=answer[i]^answer[j];
    answer[j]=answer[i]^answer[j];
    answer[i]=answer[i]^answer[j];
  }
  return answer;
}
