#ifndef _DIRSCANNER_H_
#define _DIRSCANNER_H_

#include "../include/configuration.h"


class DirScanner
{
public:
    DirScanner(Configuration & conf);
    void operator()();
    vector<string> & getFilesPath();
    void traverse(const string &dirname);

private:
    vector<string> _vecFilesPath; //每个预料文件的绝对路径
    Configuration & _conf;


};

#endif
