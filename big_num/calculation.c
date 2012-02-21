#include "calculation.h"

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

char **devide(char *a,char*b){
  char *answer[2]; 
  char *temp;
  char devider[MAX_DEVIDER]="0";
  long j=0,k=0;
  long len_a;
  len_a=strlen(a);
  answer[1]=(char *)malloc(len_a*sizeof(char)); 
  strcpy(answer[1],a);
  k=0;
  answer[0]=(char *)malloc(len_a*sizeof(char)); 
  memset(answer[0],'0',len_a);
  answer[0][len_a]=0;
  temp=(char *)malloc(len_a*sizeof(char));
  temp[len_a]=0;
  while(compare(answer[1],b)>=0 ||temp[0]!=0 || answer[1][0]=='0'){
    temp[k++]=answer[1][0];
    temp[k]=0;

    *devider='0';
    while(compare(temp,multiple(b,devider))>=0)
      devider[0]++;
    devider[0]--;
    if(compare(temp,multiple(b,devider))==0){
      answer[0][j++]=devider[0];
      answer[1]++;
      temp[0]=0;
      k=0;
    }
    else if(devider[0]=='0'){
      answer[1]++;
      continue;
    }
    else{
      answer[0][j++]=devider[0];
      temp=minus(temp,multiple(b,devider));
      answer[1]++;
      answer[1]-=strlen(temp);
      for(k=strlen(temp)-1;k>=0;k--)
	answer[1][k]=temp[k];
      temp[++k]=0;
    }
  }
 /* for(i=0;i<strlen(answer[1]);i++)
    answer[0][j++]='0';*/
  answer[0][j]=0;
  if(answer[1][0]==0)
    answer[1]="0";
  return answer; 
}

short int compare(char *a,char *b){
  long len_a,len_b;
  len_a=strlen(a);
  len_b=strlen(b);
  if(len_a>len_b)
    return 1;
  else if(len_b>len_a)
    return -1;
  else return strcmp(a,b);
}
