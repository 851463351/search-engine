#include "Socket.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

Socket::Socket()
{
    _fd=::socket(AF_INET,SOCK_STREAM,0);
    if(-1==_fd)
    {
        perror("socket");
    }
}

Socket::Socket(int fd)
    :_fd(fd) {}

int Socket::fd() const
{
    return _fd;
}

void Socket::shutdownWrite()
{
    ::shutdown(_fd,SHUT_WR);
}

Socket::~Socket()
{
    ::close(_fd);
}
