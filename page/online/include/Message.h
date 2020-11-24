#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
struct Message
{
    int _id;
    int _len;
    /* std::string _content; */
    char _content[65536];
};

#endif
