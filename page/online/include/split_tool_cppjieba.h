#ifndef __SPLITTOOLCPPJIEBA_H__
#define __SPLITTOOLCPPJIEBA_H__

#include "../lib/cppjieba/include/Jieba.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;

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

