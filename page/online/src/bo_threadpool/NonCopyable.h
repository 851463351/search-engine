#ifndef _NONCOPY_H_
#define _NONCOPY_H_


class NonCopyable
{
protected:
    NonCopyable() {}
    ~NonCopyable() {}

    NonCopyable(const NonCopyable &)=delete ;
    NonCopyable & operator=(const NonCopyable &)=delete ;

};

#endif
