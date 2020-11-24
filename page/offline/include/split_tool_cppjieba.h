#ifndef __SPLITTOOLCPPJIEBA_H__
#define __SPLITTOOLCPPJIEBA_H__

#include "split_tool.h"
#include "../lib/cppjieba/include/Jieba.hpp"


class SplitToolCppJieba
{
private:
    bool isChinese(const string &);
public:
    static vector<string> cut(const string &sentence);
private:
    static cppjieba::Jieba _jieba;
};
#endif

