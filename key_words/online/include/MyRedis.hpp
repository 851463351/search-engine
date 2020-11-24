#pragma once
#include "sw/redis++/redis++.h"

class MyRedis
{
public:
    static sw::redis::Redis* getInstance()
    {
        if(nullptr == _pRedis)
        {
            sw::redis::ConnectionOptions con;
            sw::redis::ConnectionPoolOptions con_pool;
            con.db = 0;
            con.host = "127.0.0.1";
            con.port = 6379;
            con_pool.size = 3;
            _pRedis = new sw::redis::Redis(con, con_pool);
            atexit(destroy);
        }
        return _pRedis;
    }

    static void destroy()
    {
        if(_pRedis)
        {
            delete _pRedis;
            _pRedis = nullptr;
        }
    }

private:
    MyRedis() {}
    ~MyRedis() {}

private:
    static sw::redis::Redis *_pRedis;
};

sw::redis::Redis* MyRedis::_pRedis = getInstance(); //采用饱汉模式，多线程安全

