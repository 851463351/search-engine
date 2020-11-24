#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"

#include <sys/eventfd.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <iostream>

EventLoop::EventLoop(Acceptor &acceptor)
    :_efd(createEpollfd())
    ,_acceptor(acceptor)
    ,_evtList(1024)  //初始化epoll数组
    ,_isLooping(false)
    ,_eventfd(createEventfd())
{
    addEpollReadfd(_acceptor.fd()); //监听文件描述符注册epoll事件i
    addEpollReadfd(_eventfd); //用户通信的_eventfd添加监听
}

void EventLoop::loop()
{
    _isLooping=true;
    while(_isLooping)
    {
        waitEpollfd();
    }
}

void EventLoop::unloop()
{
    if(_isLooping)
    {
        _isLooping=false;
    }
}

void EventLoop::runInloop(Functor && cb)
{
    {
        MutexLockGuard mutex(_mutex);
        _pendingFunctors.push_back(std::move(cb));
    }

    wakeup();  //唤醒IO线程
}

void EventLoop::setConnectionCallBack(TcpConnectionCallBack &&cb)
{
    _onConnection=std::move(cb);
}
void EventLoop::setMessageCallBack(TcpConnectionCallBack && cb)
{
    _onMessage=std::move(cb);
}

void EventLoop::setCloseCallBack(TcpConnectionCallBack && cb)
{
    _onClose=std::move(cb);
}

void EventLoop::waitEpollfd()
{
    int nready=0;
    do{
        
        nready=::epoll_wait(_efd,&_evtList[0],_evtList.size(),5000); //5s超时
    }while(-1==nready && errno==EINTR);
    
    if(-1==nready)
    {
        perror("epoll_wait");
        return;
    }
    else if(0==nready)
    {
        printf(">>epoll_wait timeout !\n");
    }else
    {
        if(nready==(int)_evtList.size())
        {
            _evtList.resize(2*nready);
        }

        for(int idx=0;idx!=nready;++idx)
        {
            int fd=_evtList[idx].data.fd;
            if(fd==_acceptor.fd())
            //监听套接字就绪（有新连接）
            {
                if(_evtList[idx].events & EPOLLIN)
                {
                    handleNewConnection();
                }
            }
            else if(fd==_eventfd) 
            {
                //计算线程通知IO线程处理事情
                if(_evtList[idx].events & EPOLLIN)
                {
                    handleRead();
                    doPendingFunctors();
                }
            }
            else
            {
                //处理已建立的连接
                if(_evtList[idx].events&EPOLLIN)
                {
                    handleMessage(fd);
                }
            }
        }

    }
}

int EventLoop::createEpollfd()
{
    int ret=::epoll_create1(0);
    if(-1==ret)
    {
        perror("epoll_create1");
    }
    return ret;
}

void EventLoop::addEpollReadfd(int fd)
{
    struct epoll_event value;
    value.data.fd=fd;
    value.events=EPOLLIN;
    int ret=::epoll_ctl(_efd,EPOLL_CTL_ADD,fd,&value);
    if(-1==ret)
    {
        perror("epoll_ctl");
    }
}

void EventLoop::delEpollReadfd(int fd)
{
    struct epoll_event value;
    value.data.fd=fd;
    int ret=::epoll_ctl(_efd,EPOLL_CTL_DEL,fd,&value);
    if(-1==ret)
    {
        perror("epoll_ctl");
    }
}

void EventLoop::handleNewConnection()
{
    int peerfd=_acceptor.accept();

    addEpollReadfd(peerfd);

    TcpConnectionPtr conn(new TcpConnection(peerfd,this));

   conn->setConnectionCallBack(_onConnection);
   conn->setMesssageCallBack(_onMessage);
   conn->setCloseCallBack(_onClose);

   _conns.insert(std::make_pair(peerfd,conn));
   conn->handleConnectionCallBack();
}

void EventLoop::handleMessage(int fd)
{
    auto iter=_conns.find(fd);
    assert(iter!=_conns.end());

    //判断链接是否断开
    bool isClosed=iter->second->isConnectionClose();
    if(isClosed)
    {
        delEpollReadfd(fd);
        iter->second->handleCloseCallBack();
        _conns.erase(iter);
    }
    else{
        iter->second->handleMessageCallBack();
    }

}

int EventLoop::createEventfd()
{
    int ret=::eventfd(0,0);
    if(-1==ret)
    {
        perror("eventfd");
    }
    return ret;
}

void EventLoop::handleRead()
{
    uint64_t howmany=0;
    int ret=::read(_eventfd,&howmany,sizeof(howmany));
    if(ret!=sizeof(howmany))
    {
        perror("read");
    }
}

void EventLoop::wakeup()
{
    uint64_t one=1;
    int ret=::write(_eventfd,&one,sizeof(one));
    if(ret!=sizeof(one))
    {
        perror("write");
    }
}

void EventLoop::doPendingFunctors()
{
    vector<Functor> temp;
    {
        MutexLockGuard muetx(_mutex);
        temp.swap(_pendingFunctors);
    }

    for(auto & functor:temp)
    {
        functor();
    }
}
