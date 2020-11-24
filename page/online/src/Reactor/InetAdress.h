#ifndef _INETADRESS_H_
#define _INETADRESS_H_

#include <arpa/inet.h>
#include <string>
using std::string;

class InetAdress
{
public:
    InetAdress(unsigned short port);
    InetAdress(const string & ip,unsigned short port);
    InetAdress( const struct sockaddr_in  &addr);

    string ip() const;
    unsigned short port() const;
    struct sockaddr_in * getInetAdressPtr(); 

private:
    struct sockaddr_in _addr;

};

#endif
