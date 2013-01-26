#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<time.h>
#define filenm "./msg"
#define reAsOn 2
#define Napkin 1

struct msgmbuf
{
	long msg_type;
	char msg_text[512];
};
time_t rawtime;
struct tm* timeinfo;
char ch[15];

int main(){
	int qid;
	key_t key;
	int len;
	struct msgmbuf msg;
	if ((key = ftok(filenm,'a'))==-1){
		perror("Generate standard error!");
		exit(1);
	}
	if ((qid = msgget(key,0))==-1){
		perror("Create new message line wrong!");
		exit(1);
	}
    /*
	if ((qid = msgget(key,IPC_CREAT|0666))==-1){
		perror("Create new message line wrong!");
		exit(1);
	}
    */
	printf("This is Napkin:\n");
	while (1){
		if ((msgrcv(qid,&msg,512,reAsOn,0))<0){
			perror("reAsOn is logging off");
			exit(1);
		}
		printf("reAsOn:%s\n",(&msg)->msg_text);	
		printf("Napkin:");
		fgets((&msg)->msg_text,512,stdin);
		time(&rawtime);
    	timeinfo = localtime(&rawtime);
    	sprintf(ch,"(%d:%d:%d)",(int)(timeinfo->tm_hour),(int)(timeinfo->tm_min),(int)(timeinfo->tm_sec));
		len = strlen(msg.msg_text);
		msg.msg_text[len-1]=' ';
		strcat(msg.msg_text,ch);
		len = strlen(msg.msg_text);
		msg.msg_text[len] = '\0';
		msg.msg_type = Napkin;
		if ((msgsnd(qid,&msg,len+1,0))<0){
			perror("Add message failed!");
			exit(1);
		}
		if (strcmp((&msg)->msg_text,"88\n")==0) break;
	}
    return 0;
}
