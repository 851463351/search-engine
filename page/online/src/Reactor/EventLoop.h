#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include "../bo_threadpool/MutexLock.h"
#include <sys/epoll.h>

#include <vector>
#include <map>
#include <memory>
#include <functional>
using std::vector;
using std::map;
using std::shared_ptr;

class Acceptor;
class TcpConnection;

using TcpConnectionPtr=shared_ptr<TcpConnection>;
using TcpConnectionCallBack=std::function<void(const TcpConnectionPtr &)>;
using Functor=std::function<void()>;

class EventLoop
{
public:
    EventLoop(Acceptor & acceptor);
    
    void loop();
    void unloop();

    void runInloop(Functor && cb);//将注册的回调函数添加到vector,并唤醒IO线程

    void setConnectionCallBack(TcpConnectionCallBack && cb);
    void setMessageCallBack(TcpConnectionCallBack && cb);
    void setCloseCallBack(TcpConnectionCallBack &&cb);
private:
    int createEpollfd();
    void addEpollReadfd(int fd);
    void delEpollReadfd(int fd);

    void waitEpollfd();
    void handleNewConnection();
    void handleMessage(int fd);
    bool isConnectionClose(int fd);

    int createEventfd();
    void handleRead();
    void wakeup();
    void doPendingFunctors();
private:
    int _efd; //epoll句柄
    Acceptor & _acceptor;  //会用到Acceptor里面的监听套接字和accept
    vector<struct epoll_event> _evtList;
    map<int,TcpConnectionPtr> _conns;  //peerfd, shared_ptr<TcpConnection>
    bool _isLooping;

    int _eventfd; //计算线程和IO线程间通信
    std::vector<Functor> _pendingFunctors; //存储注册的回调IO线程send函数的函数
    MutexLock _mutex;

    TcpConnectionCallBack _onConnection;
    TcpConnectionCallBack _onMessage;
    TcpConnectionCallBack _onClose;
};

#endif
