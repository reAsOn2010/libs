
  #include "stdio.h"  
  #include "string.h"  
  #include "stdlib.h"  

  char *minus(char *a,char*b);  

  int main(int argc,char *argv[]){  
    char *answer;  
    if(argc!=3){  
      printf("input 2 numbers\n");  
      return -1;  
    }  
    answer=minus(argv[1],argv[2]);  
    printf("%s\n",answer);  
    return 0;  
  }  

/*minus函数参数为两个字符串，表示参与减法所需的两个数
函数返回一个字符串地址，里面存储了字符串形式的所减值的地址
其中，第一个数为被减数，第二个为减数*/
char *minus(char *a,char *b){
  long len_a=0,len_b=0;
  char *p;
  short int back=0/*退位*/,temp=0,sign=0/*符号*/;
  long i=0,j=0;
  char *answer; 
  len_a=strlen(a);
  len_b=strlen(b);
  /*确保a>b*/
  if(strcmp(a,b)==0)return "0";
  if(len_a<len_b||((len_a==len_b)&&(strcmp(a,b)<0))){
    len_a=len_a^len_b;
    len_b=len_a^len_b;
    len_a=len_a^len_b;
    p=a;
    a=b;
    b=p;
    sign=-1;
  }
  answer=(char *)malloc((len_a+1)*sizeof(char));
  for(i=len_b-1;i>=0;i--){
    temp=a[i+len_a-len_b]-'0'-back;
    back=0;
    if(temp<b[i]-'0'){
      back=1;
      temp+=10;
    }
    temp-=(b[i]-'0');
    answer[j++]=temp+'0';
  }
  i=len_a-len_b-1;
  if(len_a>len_b){
    while(i>0){
      a[i]-=back;
      back=0;
      if(a[i]<'0'){
	a[i]+=10;
	back=1;
      }
      answer[j++]=a[i--];
    }
    if((a[0]-back)!='0')
      answer[j++]=a[0]-back;
  }
  while(answer[j-1]=='0')j--;
  if(sign)
    answer[j++]='-';
  answer[j]=0;
  for(i=strlen(answer)-1,j=0;j<(strlen(answer)/2);i--,j++){
    answer[i]=answer[i]^answer[j];
    answer[j]=answer[i]^answer[j];
    answer[i]=answer[i]^answer[j];
  }
  return answer; 
}
