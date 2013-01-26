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

	if ((qid = msgget(key,IPC_CREAT|0666))==-1){
		perror("Create new message line wrong!");
		exit(1);
	}

	printf("This is reAsOn:\n");

	printf("reAsOn:");
	fgets((&msg)->msg_text,512,stdin);

	while (strcmp((&msg)->msg_text,"88\n")!=0){
		time(&rawtime);
    	timeinfo = localtime(&rawtime);
    	sprintf(ch,"(%d:%d:%d)",(int)(timeinfo->tm_hour),(int)(timeinfo->tm_min),(int)(timeinfo->tm_sec));
		len = strlen(msg.msg_text);
		msg.msg_text[len-1]=' ';
		strcat(msg.msg_text,ch);
		msg.msg_type = reAsOn;
		
		len = strlen(msg.msg_text);

		msg.msg_text[len]='\0';
		if ((msgsnd(qid,&msg,len+1,0))<0){
			perror("Add message failed!");
			exit(1);
		}

		if ((msgrcv(qid,&msg,512,Napkin,0))<0){
			perror("Read message failed!");
			exit(1);
		}

		printf("Napkin:%s\n",(&msg)->msg_text);
		
		printf("reAsOn:");
		fgets((&msg)->msg_text,512,stdin);
		
	}

	if ((msgctl(qid,IPC_RMID,NULL))<0){
        perror("delete message failed");
        exit(1);
    }
    exit(0);
}
