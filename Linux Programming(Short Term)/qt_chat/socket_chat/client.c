#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for sleep()
#include <arpa/inet.h> //for inet_addr()

#define MAX_USER 10
#define MAX_PENDING_CONNECTS 10
#define PORT 9090

void get(int sockfd);
char recv_buf[1046];
char send_buf[1024];
int main(int argc, char *argv[])
{
    pthread_t pid;
    int sockfd;
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if(argc == 1)
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //localhost
    else if(argc == 2)
        server_addr.sin_addr.s_addr = inet_addr(argv[argc - 1]);
    else
        printf("Parameter Error!");

    //建立客户端套接字
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("client socket init error!");
        perror("client socket init error!");
        exit(1);
    }
    printf("Input \"bye\" to exit\n",sockfd);

    //连接服务器
    if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect error!");
        exit(1);
    }

    char name[20];
    printf("Input your name:");
    scanf("%s", name);
    send(sockfd, name, strlen(name), 0);
    pthread_create(&pid, NULL, (void* (*)(void*))get, (void *)sockfd);

    while(1)
    {
        gets(send_buf);
        if(send(sockfd, send_buf, strlen(send_buf), 0) == -1)
        {
            perror("send message error!");
            exit(1);
        }
        if(strcmp(send_buf, "bye") == 0)
        {
            break;
        }
        sleep(1);
    }

    close(sockfd);
    pthread_cancel(pid);
    return 0;
}

void get(int sockfd)
{
    int recv_len;
    while(1)
    {
        if((recv_len = recv(sockfd, recv_buf, 1046, 0)) == -1)
        {
            perror("client receive data error!");
            exit(1);
        }
        else{
            recv_buf[recv_len] = '\0';
            printf("%s\n", recv_buf);
        }
    }
}
