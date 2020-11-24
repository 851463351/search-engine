#ifndef __WD_SOCKETIO_HPP__
#define __WD_SOCKETIO_HPP__
#include <string>

struct Message
{
    Message()
    : _id(0)
    , _len(0)
    , _content()
    {}

    int _id;
    int _len;
    char _content[128] = {0};
};

namespace wd
{

class SocketIO
{
public:
    explicit
    SocketIO(int fd);

    int readn(char *buf, int len);
    int readline(char *buf, int maxlen);    //每次读取一行数据
    int writen(const Message *buf, int len);

private:
    int recvPeek(char *buf, int len);

private:
    int _fd;
};

}//end of namespace wd

#endif
