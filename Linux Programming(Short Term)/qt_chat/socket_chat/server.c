#include<stdio.h>
#include<stdlib.h> //for perror()

#include<sys/types.h>
#include<sys/socket.h> //for socket operations
#include<pthread.h> //for multi-thread programming
#include<netinet/in.h> //for struct sockaddr_in
#include<string.h> //for bzero()
#include <unistd.h> //for close()


#define MAX_USER 10
#define MAX_PENDING_CONNECTS 10
#define PORT 9090
// int socket(int af, int type, int protocol)

int socket_i[MAX_USER];
void broadcast(int client_fd);

int main(int argc, char *argv[])
{
    int i;
    for(i = 0; i < MAX_USER; i++)
    {
        socket_i[i] = -1;
    }

    pthread_t pid;
    int sockfd, client_fd;
    socklen_t sin_size;
    struct sockaddr_in server_addr, remote_addr;

    //建立服务端套接字
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("server socket init error!");
        exit(1);
    }
    printf("server sockfd %d\n", sockfd);

    //服务端配置
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY = 0x00000000
    bzero(&(server_addr.sin_zero),8); //set sin_zero[8] to 0

    //绑定socket到端口
    if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind error!");
        exit(1);
    }

    //监听
    if(listen(sockfd,MAX_PENDING_CONNECTS) == -1)
    {
        perror("listen error!");
        exit(1);
    }

    i = 0;
    //接受链接请求
    while(1){
        sin_size = sizeof(struct sockaddr_in);
        if((client_fd = accept(sockfd, (struct sockaddr*)&remote_addr, &sin_size)) == -1)
        {
            perror("accept error!");
            exit(1);
        }
        else
            printf("client sockfd %d\n", client_fd);
    
        //找到空的socket位置
        while(socket_i[i] != -1)
            i = (i+1) % MAX_USER;

        //保存socket
        socket_i[i] = client_fd;
        printf("before pthread\n");
        pthread_create(&pid, NULL, /*(void* (*)(void*))*/(void *)broadcast, (int *)client_fd);
        printf("after pthread\n");
        printf("client fd %d loged in\n",client_fd);
    }
}

void broadcast(int client_fd)
{
    char message[1046];
    char buf[1024];
    int i, recv_len;
    char name[20];
    if((recv_len = recv(client_fd, name, 20, 0)) == -1)
    {
        perror("server receive name error!");
        exit(1);
    }
    name[recv_len]  = ':';
    name[recv_len + 1] = '\0';

    printf("new thread");
    while(1){
        printf("before receive");
        if((recv_len = recv(client_fd, buf, 1024, 0)) == -1)
        {
            perror("server receive data error!");
            exit(1);
        }
        buf[recv_len] = '\0';
        printf("after receive");
        if(strcmp(buf, "bye") == 0)
        {
            printf("%s goodbye!\n", name);
            break;
        }
        else{
            for(i = 0; i < MAX_USER; i++)
            {
                if(socket_i[i] == -1)
                    continue;
                else{
                    message[0] = '\0';
                    strcat(message, name);
                    strcat(message, buf);
                    if(send(socket_i[i], message, strlen(message), 0) == -1)
                    {
                        perror("send message error!");
                        exit(1);
                    }
                }
            }
        }
        printf("after broadcast");
    }

    for(i = 0; i < MAX_USER; i++)
    {
        if(socket_i[i] == client_fd)
            socket_i[i] = -1;
    }
    close(client_fd);
    pthread_exit(NULL);
}
