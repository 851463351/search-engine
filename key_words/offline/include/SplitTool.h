#ifndef SEARCH_ENGINE_SRC_SPLITTOOL_H_
#define SEARCH_ENGINE_SRC_SPLITTOOL_H_

#include <string>
#include <vector>
using std::vector;
using std::string;

class SplitTool 
{
public:
    SplitTool() = default;
    virtual vector<string> cut(const string &sentence) = 0;
};

#endif 
