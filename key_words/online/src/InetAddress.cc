#include "../include/InetAddress.hpp"
#include <string.h>

namespace wd
{

InetAddress::InetAddress(unsigned short port)
{
    ::memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);   //主机字节序转网络字节序（小端 -> 大端）
    _addr.sin_addr.s_addr = INADDR_ANY; //相当于inet_addr("0.0.0.0")，表示本机所有网卡IP地址
}

InetAddress::InetAddress(const string &ip, unsigned short port)
{
    ::memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);   
    _addr.sin_addr.s_addr = inet_addr(ip.c_str()); 
}

InetAddress::InetAddress(const struct sockaddr_in &addr)
: _addr(addr)
{}

string InetAddress::ip() const
{
    return string(::inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port() const
{
    return ntohs(_addr.sin_port);
}

}//end of namespace wd
