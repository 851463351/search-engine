#include "EchoServer.h"
#include "TcpConnection.h"
#include "../../include/WebPageQuery.h"
#include "../../include/configuration.h"

#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

/* class MyTask */
/* { */
/* public: */
/*     MyTask(const string & msg,const TcpConnectionPtr & pConn) */
/*         :_msg(msg) */
/*         ,_pConn(pConn) */
/*     {} */
    
/*     void process()   //处理具体任务 */
/*     { */
/*         _pConn->sendInloop(_msg); //处理完后的数据交给主线程,由主线程发送给对端 */
/*     } */
/* private: */
/*     string _msg; */
/*     TcpConnectionPtr _pConn; */
/* }; */

void onConnection(const TcpConnectionPtr & conn)
{
    cout<<">>"<<conn->toString()<<" has connected!"<<endl;
    /* conn->send("Welcome to Server!"); */
}

void onMessage(const TcpConnectionPtr &conn, Threadpool* threadpool, Configuration & conf)
{
    string msg=conn->receive();
    cout<<">>> receive:"<<msg<<endl;
    
    
    //交给处理任务线程
    WebPageQuery mytask(msg,conn,conf);
    threadpool->addTask(std::bind(&WebPageQuery::doQuery,mytask));    
}

void onClose(const TcpConnectionPtr &conn)
{
    cout<<">>"<<conn->toString()<<" has closed!"<<endl;
}



int main(int argc,char *argv[])
{
    EchoServer server(4,10,"192.168.218.151",2000);

    Configuration conf("../conf/path.conf");

    server.setConnectionCallBack(onConnection);
    server.setMessageCallBack(std::bind(onMessage,std::placeholders::_1,server.getThreadpool(),conf));
    server.setCloseCallBack(onClose);

    server.start();

    return 0;
}

