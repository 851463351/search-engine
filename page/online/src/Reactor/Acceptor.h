#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#include "InetAdress.h"
#include "Socket.h"

class EventLoop;
class Acceptor
{
    friend class EventLoop;
public:
    Acceptor(unsigned short port);
    Acceptor(const string & ip,unsigned short port);

    void ready();
    int  accept();
private:
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void bind();
    void listen();

    int fd() const;
private:
   InetAdress _addr;
   Socket _listensock;

};

#endif
