#ifndef _SOCKET_H_
#define _SOCKET_H_


class Socket
{
public:
    Socket();
    Socket(int fd);

    int fd() const;

    void shutdownWrite(); //关闭写端

    ~Socket();

private:
    int _fd;

};

#endif
