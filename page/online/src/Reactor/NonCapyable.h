#ifndef _NONCAPYABLE_H_
#define _NONCAPYABLE_H_


class NonCapyable
{
protected:
    NonCapyable() {}
    ~NonCapyable() {}

    NonCapyable(const NonCapyable &)=delete;
    NonCapyable & operator=(const NonCapyable &)=delete;

private:

};

#endif
