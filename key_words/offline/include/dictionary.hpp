#pragma once
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <fstream>
#include <iostream>
using namespace std;

class Configuration
{
public:
    Configuration(const string &filepath)
    {
        char absolute_path[128] = {0};
        realpath(filepath.c_str(), absolute_path);
        _filepath = absolute_path;
    
        cout << _filepath << endl;

        ifstream ifs(_filepath);
        string line;

        vector<string> words = {"enConfig", "cnConfig", "enStop", "cnStop"};
        for(int i = 0; i < 4; ++i)
        {
            getline(ifs, line);
            bzero(&absolute_path, sizeof(absolute_path));
            realpath(line.c_str(), absolute_path);
            _configMap[words[i]] = absolute_path;
        }
    }

    map<string, string>& getConfigMap()
    {
        return _configMap;    
    }

private:
    string _filepath;
    map<string, string> _configMap;
};

class SplitTool;

class DictProducer
{
public:
    DictProducer(const string &dir);    //英文语料目录构造函数
    DictProducer(const string &dir, SplitTool *splitTool);  //中文语料目录构造函数
    void buildEnDict(const string &stopWordsPath);
    void buildEnIndex();
    void buildCnDict();
    void buildCnIndex();
    void storeDict(const char *filepath);
    void storeIdx(const char *filepath);
    void showFiles() const
    {
        for(auto &elem : _files)
        {
            cout << elem << endl;   
        }
    }

    void showDict() const;
    void showIndex() const;
    void getFiles();

private:
    string _dir;
    vector<string> _files;
    map<string, int> _dict; //词典
    unordered_map<string, set<int>> _index; //索引
    SplitTool *_splitTool;
};
