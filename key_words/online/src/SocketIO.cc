#include "../include/SocketIO.hpp"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace wd
{

SocketIO::SocketIO(int fd)
: _fd(fd)
{}

int SocketIO::readn(char *buf, int len)
{
    int left = len;
    char *p = buf;
    int ret;
    while(left > 0)
    {
        ret = ::read(_fd, p, left);
        if(ret == -1 && errno == EINTR) //EINTR为中断错误
            continue;
        else if(ret == -1)
        {
            perror("read");
            return len - left;
        }
        else if(ret == 0)   //客户端断开
            return len - left;
        else
        {
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

int SocketIO::recvPeek(char *buf, int len)  //从内核缓冲区复制到用户缓冲区，内核缓冲区不移走数据 
{
    int ret;
    do
    {
        ret = ::recv(_fd, buf, len, MSG_PEEK);
    } while(ret == -1 && errno == EINTR);
    return ret;
}

int SocketIO::readline(char *buf, int maxlen)
{
    int left = maxlen - 1;  //结尾是'\0'
    char *p = buf;
    int ret;
    int total = 0;
    while(left > 0)
    {
        ret = recvPeek(p, left);    
        
        //查找'\n'
        for(int i = 0; i != ret; ++i)
        {
            if(p[i] == '\n')
            {
                int sz = i + 1;
                readn(p, sz);   //从内核缓冲区取走这行数据
                total += sz;
                p += sz;
                *p = '\0';
                return total;
            }
        }

        //如果没有发现'\n'
        readn(p, ret);
        left -= ret;
        p += ret;
        total += ret;
    }
    *p = '\0';  //最终没有发现'\n'
    return total;
}

int SocketIO::writen(const Message *buf, int len)
{
    int left = len;
    const Message *p = buf;
    int ret;
    while(left > 0)
    {
        ret = ::write(_fd, p, left);
        if(ret == -1 && errno == EINTR)
            continue;
        else if(ret == -1)
        {
            perror("write");
            return len - left;
        }
        else
        {
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

}//end of namespace wd
