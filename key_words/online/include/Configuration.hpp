#pragma once
#include <string.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
using namespace std;

class Configuration
{
public:
    Configuration() 
    : _path()
    {
        vector<string> tmp = {"enDict", "enIdx", "cnDict", "cnIdx"};
        ifstream ifs("../conf/config");
        char absolute_path[128];
        string line;
        for(int i = 0; i < 4; ++i)
        {
            getline(ifs, line);
            bzero(&absolute_path, sizeof(absolute_path));
            realpath(line.c_str(), absolute_path);
            _path[tmp[i]] = absolute_path;
        }
    }
    ~Configuration() {}

public:
    map<string, string> _path; 
};

