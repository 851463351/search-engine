#include  "../include/DirScanner.h"

#include <dirent.h>
#include <sys/types.h>
#include <string.h>


DirScanner::DirScanner( Configuration & conf)
    :_conf(conf)
{}

void DirScanner::traverse(const string & dirname)
{

    DIR *pdir=opendir(dirname.c_str());
    if(pdir==nullptr)
    {
        perror("opendir");
        return;
    }
    struct dirent * pdirent;
    while(pdirent=readdir(pdir))
    {
        if(strcmp(pdirent->d_name,".")==0||strcmp(pdirent->d_name,"..")==0)
        {
            continue;
        }
        string filePath;
        filePath=dirname+"/"+pdirent->d_name;
        _vecFilesPath.push_back(filePath);
    }
}

void DirScanner::operator()()
{
    traverse(_conf.getConfigMap()["rss_path"]);
}

vector<string> & DirScanner::getFilesPath()
{
    return _vecFilesPath;
}

