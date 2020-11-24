#ifndef __WD_EVENTLOOP_H__
#define __WD_EVENTLOOP_H__

#include "MutexLock.hpp"
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>

namespace wd
{

class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
using Functor = std::function<void()>;

class Acceptor;

class EventLoop
{
public:
    EventLoop(Acceptor &);
    
    void loop();
    void unloop();

    void runInLoop(Functor &&cb);

    void setConnectionCallback(TcpConnectionCallback &&cb)
    {   _onConnection = std::move(cb);  }
    void setMessageCallback(TcpConnectionCallback &&cb)
    {   _onMessage = std::move(cb);  }
    void setCloseCallback(TcpConnectionCallback &&cb)
    {   _onClose = std::move(cb);  }

private:
    int createEpollfd();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);

    void waitEpollfd();
    
    void handleNewConnection();
    void handleMessage(int fd);

    int createEventfd();
    void handleRead();
    void wakeup();
    void doPendingFunctors();

private:
    int _efd;
    int _eventfd;
    Acceptor &_acceptor;
    std::vector<struct epoll_event> _evtList;
    std::map<int, TcpConnectionPtr> _conns;
    bool _isLooping;

    std::vector<Functor> _pendingFunctors;
    MutexLock _mutex;

    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
};

}//end of namespace wd

#endif
