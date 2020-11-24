#include "SocketIO.h"

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>

using std::cout;
using std::endl;

SocketIO::SocketIO(int fd)
    :_fd(fd) {}

int SocketIO::readn(char * buff,int len)
{
    int left=len;
    char *p=buff;
    while(left>0)
    {
        int ret=::read(_fd,p,left);

        if(-1==ret && errno==EINTR)
        {
            continue;
        }
        else if(-1==ret)
        {
            perror("read");
            return len-left;
        }
        else if(ret==0)
        {
            return len-left;
        }
        else
        {
            left-=ret; //下次还需要读的长度
            p+=ret; //移动指针
        }
    }
    return len-left;  //总共要读的长度-减去还要读的长度=已读的长度
}

int SocketIO::readline(char *buff,int maxlen)
{
    int left=maxlen-1;
    char *p=buff;
    int ret;
    int total=0;

    while(left>0)
    {
        ret=recvPeek(p,left);
        //查找“\n”
        for(int idx=0;idx<ret;++idx)
        {
            if(p[idx]=='\n')
            {
                int sz=idx+1;
                readn(p,sz);
                total+=sz;
                p+=sz;
                *p='\0';
                return total;
            }
        }
        //如果没有发现‘\n’
        readn(p,ret);
        left-=ret;
        p+=ret;
        total+=ret;
    }

    *p='\0';
    return total;
}

int SocketIO::recvPeek(char *buff,int len)
{
    int ret;
    do{
        ret=::recv(_fd,buff,len,MSG_PEEK);
    }while(ret==-1 &&errno==EINTR);
    
    return ret;
}

int SocketIO::writen(const Message * buff,int len)
{
    int left=len;
    const Message *p=buff;
    while(left>0)
    {
        int ret=::write(_fd,p,left);
        /* std::cout<<"SocketIO::ret="<<ret<<endl; */
        if(-1==ret &&errno==EINTR)
        {
            continue;
        }
        else if(-1==ret)
        {
            perror("write");
            return len-left;
        }
        else
        {
            left-=ret;
            p+=ret;
        }
    }
    return len-left;
}
