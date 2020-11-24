#include "../include/PageLib.h"
#include "../include/DirScanner.h"
#include "../include/FileProcessor.h"
#include "../include/configuration.h"
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::cin;
using std::ofstream;

PageLib::PageLib(Configuration &conf,DirScanner &dirScanner,FileProcessor &fileProcessor)
:_conf(conf)
,_dirScanner(dirScanner)
,_fileProcessor(fileProcessor)
{}

//创建网页库及偏移库
void PageLib::create()
{
    int start=0;
    int docid=1;
    for(size_t i=0;i!=_dirScanner.getFilesPath().size();++i)
    {
        /* cout<<"_dirScanner.getFilesPath="<<_dirScanner.getFilesPath()[i]<<endl; */
        _fileProcessor.parse_xml(_dirScanner.getFilesPath()[i],_vecFilesFiles,docid,start,_offsetLib);
    }
}

//存储网页库以及偏移库
void PageLib::store()
{
    string pagePath,offsetPath;
    pagePath=_conf.getConfigMap()["ripepage_path"];
    offsetPath=_conf.getConfigMap()["offset_path"];

    ofstream out1(pagePath);
    if(!out1.good())
    {
        std::cout<<"the ofstream "<<pagePath<<" opens failed"<<std::endl;
        return;
    }
    for(auto &file:_vecFilesFiles)
    {
        out1<<file<<endl<<endl<<endl;
    }
    ofstream out2(offsetPath);
    if(!out2.good())
    {
        std::cout<<"the ofstream "<<offsetPath<<" opnes failed" <<std::endl;
        return;
    }
    for(auto &offset:_offsetLib)
    {
        out2<<offset.first<<" "<<offset.second.first<<" "<<offset.second.second<<std::endl;
    }
}
