
  #include "stdio.h"  
  #include "string.h"  
  #include "stdlib.h"  

  char *multiple(char *a,char *b);  

  int main(int argc,char *argv[]){  
    char *answer;  
    if(argc!=3){  
      printf("please input 2 numbers\n");  
      return -1;  
    }  
    answer=multiple(argv[1],argv[2]);  
    printf("%s\n",answer);  
    return 0;  
 }  

char *multiple(char *a,char *b){
  char *answer;
  long a_len,b_len;
  short int mul_addup=0,add_addup=0,temp=0;
  long i,j;
  a_len=strlen(a);
  b_len=strlen(b);
  answer=(char *)malloc((a_len+b_len)*sizeof(char));
  memset(answer,'0',a_len+b_len);
    for(i=b_len-1;i>=0;i--){
      for(j=a_len-1;j>=0;j--){
	temp=(a[j]-'0')*(b[i]-'0')+mul_addup;
	mul_addup=0;
	mul_addup=temp/10;
	temp%=10;
	answer[(a_len-1-j)+(b_len-1-i)]+=(temp+add_addup);
	add_addup=0;
	if(answer[(a_len-1-j)+(b_len-1-i)]>'9'){
	  add_addup=1;
	  answer[(a_len-1-j)+(b_len-1-i)]-=10;
	}
      } 
      if(mul_addup){
	answer[(a_len-1-j)+(b_len-1-i)]+=mul_addup;
	mul_addup=0;
      }
      if(add_addup){
	answer[(a_len-1-j)+(b_len-1-i)]+=add_addup;
	add_addup=0;
	if(answer[(a_len-1-j)+(b_len-1-i)]>'9'){
	  answer[(a_len-1-j)+(b_len-1-i)]-=10;
	  answer[(a_len-1-j)+(b_len-1-i)+1]++;
	}
      } 
    }
    if(add_addup!=0)
      answer[a_len+b_len]+=1;
    j=a_len+b_len-1;
    while(answer[j--]=='0'&&j>=0);
    j++;
    if(j==0&&answer[0]=='0')
      return "0";
    answer[j+1]=0;
    if(answer[strlen(answer)-1]=='0')
      answer[strlen(answer)-1]=0;
    for(i=0,j=strlen(answer)-1;i<strlen(answer)/2;i++,j--){
      answer[i]=answer[i]^answer[j];
      answer[j]=answer[i]^answer[j];
      answer[i]=answer[i]^answer[j];
    }
  return answer;
}
