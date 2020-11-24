#pragma once
#include "Configuration.hpp"
#include <vector>
#include <set>
#include <map>
#include <iostream>
using namespace std;

class Mydict
{
public:
    static Mydict* getInstance();
    static void destroy();

    void initDict();
    void initIdx();

    vector<pair<string, int>>& getEnDict()
    {   return _enDict; }
    vector<pair<string, int>>& getCnDict()
    {   return _cnDict; }
    map<string, set<int>>& getEnIdx()
    {return _enIdx; }
    map<string, set<int>>& getCnIdx()
    {return _cnIdx; }

private:
    Mydict()
    : _conf()
    {
        initDict();
        initIdx();

        //测试
        /* int i = 0; */
        /* for(auto &elem : _enDict) */
        /* { */
        /*     cout << elem.first << " " << elem.second << endl; */
        /*     ++i; */
        /*     if(i == 10) */
        /*          break; */
        /* } */

        /* i = 0; */
        /* for(auto &elem : _cnDict) */
        /* { */
        /*     cout << elem.first << " " << elem.second << endl; */
        /*     ++i; */
        /*     if(i == 10) */
        /*          break; */
        /* } */

        /* i = 0; */
        /* for(auto &elem : _enIdx) */
        /* { */
        /*     cout << elem.first << " "; */
        /*     for(auto &num : elem.second) */
        /*     { */
        /*         cout << num << " "; */
        /*     } */
        /*     cout << endl; */
        /*     ++i; */
        /*     if(i == 1) */
        /*         break; */
        /* } */

        /* cout << endl; */
        /* cout << _cnIdx.empty() << endl; */
        /* i = 0; */
        /* for(auto &elem : _cnIdx) */
        /* { */
        /*     cout << elem.first << " "; */
        /*     for(auto &num : elem.second) */
        /*     { */
        /*         cout << num << " "; */
        /*     } */
        /*     cout << endl; */
        /*     ++i; */
        /*     if(i == 10) */
        /*         break; */
        /* } */
    }

    ~Mydict() {}

private:
    static Mydict *_pMydict;
    Configuration _conf;
    
    vector<pair<string, int>> _enDict;
    map<string, set<int>> _enIdx;
    vector<pair<string, int>> _cnDict;
    map<string, set<int>> _cnIdx;
};
