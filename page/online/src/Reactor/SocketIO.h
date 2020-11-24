#ifndef _SOCKETIO_H_
#define _SOCKETIO_H_

#include "../../include/Message.h"

class SocketIO
{
public:
    SocketIO(int );

    int readn(char *buff,int len);
    int readline(char *buff,int maxlen);
    int writen(const Message *buff,int len);
private:
    int recvPeek(char *buff,int len);
private:
    int _fd;

};

#endif
