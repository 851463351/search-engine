#ifndef __PAGELIB_H__
#define __PAGELIB_H__

#include <map>
#include <vector>
#include <string>
using std::string;
using std::vector;
using std::map;
using std::pair;

class Configuration;
class DirScanner;
class FileProcessor;

class PageLib
{
public:
    PageLib(Configuration &conf,DirScanner &dirScanner,FileProcessor &fileProcessor);
    
    void create();//创建网页库和偏移库
    void store();//存储网页库和位置偏移库
    
private:
    Configuration &_conf;//配置文件对象的引用
    DirScanner &_dirScanner;//目录扫描对象的引用
    FileProcessor &_fileProcessor;//网页文档处理器
    vector<string> _vecFilesFiles;//存放格式化之后的网页的容器
    map<int,pair<int,int>> _offsetLib;//存放每篇文档在网页库的位置信息
};

#endif

