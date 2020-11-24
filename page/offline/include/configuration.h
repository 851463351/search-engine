#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

using std::string;
using std::map;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::cout;
using std::endl;
using std::vector;
class Configuration
{
public:
    Configuration(const string & filePath)
        :_filePath(filePath)
    {
        ifstream ifs(filePath);
        if(!ifs.good())
        {
            cout<<"the ifstream "<<filePath<<" file opens error"<<endl;
            return;
        }
        string line;

        while(getline(ifs,line))
        {
            istringstream iss(line);
            string word;
            vector<string> temp;
            while(iss>>word)
            {
                temp.push_back(word);
            }
            _configMap.insert(std::make_pair(temp[0],temp[1]));
        }
        ifs.close();
    }
    
    map<string,string> & getConfigMap()//获取配置文件内容
    {
        return _configMap;
    }

private:
    string _filePath;  //配置文件路径
    map<string,string> _configMap; //配置文件内容
};

#endif
