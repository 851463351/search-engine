#include "myJson.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

struct Message
{
    int _id = 0;
    int _len = 0;
    char _content[128] = {0};
};

void do_service(int sockfd);

int main(int argc, const char *argv[])
{
    int peerfd = socket(PF_INET, SOCK_STREAM, 0);
    if(peerfd == -1)
        ERR_EXIT("socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("10.211.55.7"); //localhost
    /* addr.sin_addr.s_addr = inet_addr("127.2.0.0"); //localhost */
    //addr.sin_addr.s_addr = INADDR_ANY; //localhost
    addr.sin_port = htons(9999);
    socklen_t len = sizeof addr;
    if(connect(peerfd, (struct sockaddr*)&addr, len) == -1)
        ERR_EXIT("Connect");

	/* char buf[1024]; */
	/* memset(buf, 0, sizeof(buf)); */
	/* read(peerfd, buf, sizeof(buf)); */
	/* printf("%s\n", buf); */

    do_service(peerfd);
    return 0;
}



void do_service(int sockfd)
{
    char recvbuf[1024] = {0};
    char sendbuf[1024] = {0};
    string jsonbuf;
    json myjson;
    int ret;
    while(1)
    {
        fgets(sendbuf, sizeof sendbuf, stdin);
        /* sendbuf[strlen(sendbuf)-1] ='\0'; */
        /* jsonbuf = myjson.pakage(sendbuf); */
        /* std::cout<<"jsonbuf"<<jsonbuf<<std::endl; */
        //服务器端使用的是readline读取因此必须添加换行符。
        /* jsonbuf.append("\n"); */
        printf("sendbuf = %s", sendbuf);
        ret = write(sockfd, sendbuf, strlen(sendbuf));
        printf("send len = %d\n", ret);
        //sleep(5);

        Message msg;
        
        //read
        ret = read(sockfd, &msg._id, 4);
        printf("recv len = %d\n", ret);
        ret = read(sockfd, &msg._len, 4);
        printf("recv len = %d\n", ret);
        printf("msg._len = %d\n", msg._len);
        int nread = read(sockfd, msg._content, msg._len);
        printf("recv len = %d\n", nread);
        /* if(nread == -1) */
        /* { */
        /*     if(errno == EINTR) */
        /*         continue; */
        /*     ERR_EXIT("read"); */
        /* } */
        /* else if(nread == 0) //代表链接断开 */
        /* { */
        /*     printf("server close!\n"); */
        /*     close(sockfd); */
        /*     exit(EXIT_SUCCESS); */
        /* } */
        /* printf("recv bytes : %d\n", nread); */
        printf("receive msg : %s\n", msg._content);

        /* memset(recvbuf, 0, sizeof recvbuf); */
        /* memset(sendbuf, 0, sizeof sendbuf); */
    }
}
