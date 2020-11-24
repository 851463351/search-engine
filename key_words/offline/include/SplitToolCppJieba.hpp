#ifndef SEARCH_ENGINE_SRC_SPLITTOOLCPPJIEBA_H_
#define SEARCH_ENGINE_SRC_SPLITTOOLCPPJIEBA_H_

#include "./cppjieba/include/cppjieba/Jieba.hpp"
#include "SplitTool.h"

class SplitToolCppJieba : public SplitTool
{
public:
    static SplitToolCppJieba* getInstance()
    {   return &myInstance; }

private:
    cppjieba::Jieba _jieba;
    SplitToolCppJieba();
    ~SplitToolCppJieba() = default;
    vector<string> cut(const string &sentence) override;
    static SplitToolCppJieba myInstance;
};

#endif
