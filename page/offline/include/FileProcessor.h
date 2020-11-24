#ifndef __FILEPROCESSOR_H__
#define __FILEPROCESSOR_H__

#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;
using std::pair;
class DirScanner;
class FileProcessor
{
public:
    void parse_xml(const string &filepath,vector<string> &vecFilesfiles,int &docid,int & start,map<int,pair<int,int>> &offsetlib);
};

#endif

