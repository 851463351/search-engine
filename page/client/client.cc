#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>
#include <json/json.h>

using std::string;
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void do_service(int sockfd);

int main(int argc, const char *argv[])
{
    int peerfd = socket(PF_INET, SOCK_STREAM, 0);
    if(peerfd == -1)
        ERR_EXIT("socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.218.151"); //localhost
    /* addr.sin_addr.s_addr = inet_addr("127.2.0.0"); //localhost */
    //addr.sin_addr.s_addr = INADDR_ANY; //localhost
    addr.sin_port = htons(2000);
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


void TransleteJson(const string & lhs)
{
    Json::Reader *readerinfo=new Json::Reader(Json::Features::strictMode());
    Json::Value root;

    if(readerinfo->parse(lhs,root))
    {
        if(root.isArray())
        {
            /* std::cout<<"isArray"<<std::endl; */
            int nArrarySize=root.size();
            /* std::cout<<"nArrarySize="<<nArrarySize<<std::endl; */
            for(int idx=0;idx<nArrarySize;++idx)
            {
                int sz=root[idx].size();
                /* std::cout<<"sz="<<sz<<std::endl; */
                if(sz==0)
                {
                    string mgs=root[idx].asString();
                    std::cout<<"-->"<<mgs<<std::endl;
                }
                else
                {
                    for(int k=0;k<sz;++k)
                    {
                        string mgs=root[idx][k].asString();

                        std::cout<<"-->"<<mgs<<std::endl;

                    }
                }
            }
        }
    }
}

void do_service(int sockfd)
{
    int mgsLen=0;
    int mgsId=0;
    char recvbuf[1024] = {0};
    char sendbuf[1024] = {0};
    while(1)
    {
        fgets(sendbuf, sizeof(sendbuf), stdin);
        write(sockfd, sendbuf, strlen(sendbuf));
		//sleep(5);

        //read
        int nread=recv(sockfd,&mgsId,4,MSG_WAITALL);

        /* printf("nread= %d\n",nread); */
        /* printf("mgsId= %d\n",mgsId); */
        if(nread == -1)
        {
            if(errno == EINTR)
                continue;
            ERR_EXIT("read");
        }
        else if(nread == 0) //代表链接断开
        {
            printf("server close!\n");
            close(sockfd);
            exit(EXIT_SUCCESS);
        }
        recv(sockfd,&mgsLen,4,MSG_WAITALL);
        /* printf("mgsLen= %d\n",mgsLen); */
        
        nread=recv(sockfd,recvbuf,mgsLen,MSG_WAITALL);
        /* printf("nread= %d\n",nread); */
        /* printf("strlen(recvbuf)= %ld\n",strlen(recvbuf)); */
        /* printf("receive msg : %s\n", recvbuf); */

        string str(recvbuf);

        TransleteJson(str);
        memset(recvbuf, 0, sizeof recvbuf);
        memset(sendbuf, 0, sizeof sendbuf);
    }
}
