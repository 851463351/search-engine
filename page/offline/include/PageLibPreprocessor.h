#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__
#include "../include/split_tool_cppjieba.h"
#include <list>
#include <string>
#include <vector>
#include <unordered_map>
using std::string;
using std::unordered_map;
using std::pair;
using std::list;
using std::vector;

class Configuration;
class WebPage;
class PageLibPreprocessor
{
public:
    PageLibPreprocessor(Configuration &conf);
    
    void doProcess();//执行预处理
private:
    void readInfoFromFile();//根据配置信息读取网页库和位置偏移库 的内容
    void cutRedundantPages();//对冗余的网页进行去重
    void buildInvertIndexTable();//创建倒排索引表
    void storeOnDisk();//将经过处理之后的网页库、位置偏移库和倒排索引表写回到磁盘
private:
    Configuration &_conf;//配置文件对象的引用
    list<WebPage> _pageLib;//网页库的容器对象
    unordered_map<int,pair<int,int>> _offsetLib;//网页库偏移对象
    unordered_map<string,vector<pair<int,double>>> _invertIndexTable;//倒排索引表对象
    SplitToolCppJieba _split;
};

#endif

