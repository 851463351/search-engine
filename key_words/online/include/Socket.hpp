#ifndef __WD_SOCKET_HPP__
#define __WD_SOCKET_HPP__

namespace wd
{

class Socket
{
public:
    Socket();
    explicit
    Socket(int fd);

    int fd() const;

    void shutdownWrite();

    ~Socket();

private:
    int _fd;
};

}//end of namespace wd

#endif
