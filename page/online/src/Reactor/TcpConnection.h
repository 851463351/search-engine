#ifndef _TCPCONNECTION_H_
#define _TCPCONNECTION_H_

#include "Socket.h"
#include "InetAdress.h"
#include "SocketIO.h"
#include "NonCapyable.h"
#include "EventLoop.h"
#include "../../include/Message.h"

class EventLoop;

class TcpConnection
:public NonCapyable
,public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int fd ,EventLoop *loop); 
    ~TcpConnection();

    string receive();
    void send(const Message & msg);
    void sendInloop(const Message & msg);

    string toString() const;
    void shutdown();

    void setConnectionCallBack(const TcpConnectionCallBack & );
    void setMesssageCallBack(const TcpConnectionCallBack &);
    void setCloseCallBack(const TcpConnectionCallBack &);

    void handleConnectionCallBack();
    void handleMessageCallBack();
    void handleCloseCallBack();

    bool isConnectionClose();
private:
    InetAdress getLocalAddr(int fd);
    InetAdress getPeerAddr(int fd);
private:
   Socket _sock; //peerfd
   SocketIO _socketIO;
   InetAdress _localAddr;
   InetAdress _peerAddr;
   bool _isShutdownWrite;

   EventLoop * _loop;

   TcpConnectionCallBack _onConnection;
   TcpConnectionCallBack _onMessage;
   TcpConnectionCallBack _onClose;

};

#endif
