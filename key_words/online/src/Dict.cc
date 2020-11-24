#include "../include/Dict.hpp"
#include <stdlib.h>
#include <sstream>
using namespace std;

Mydict* Mydict::_pMydict = nullptr;

Mydict* Mydict::getInstance()
{
    if(nullptr == _pMydict)
    {
        _pMydict = new Mydict;
        atexit(destroy);
    }
    return _pMydict;
}

void Mydict::destroy()
{
    if(_pMydict)
    {
        delete _pMydict;
        _pMydict = nullptr;
    }
}

void Mydict::initDict()
{
    string line;
    string word;
    int num;
    ifstream ifs1(_conf._path["enDict"]);
    while(getline(ifs1, line))
    {
        istringstream iss1(line);
        while(iss1 >> word)
        {
            iss1 >> num;
            _enDict.push_back(make_pair(word, num));
        }
    }
    ifs1.close();

    ifstream ifs2(_conf._path["cnDict"]);
    while(getline(ifs2, line))
    {
        istringstream iss2(line);
        while(iss2 >> word)
        {
            iss2 >> num;
            _cnDict.push_back(make_pair(word, num));
        }
    }
    ifs2.close();
}

void Mydict::initIdx()
{
    string line;
    string word;
    int num;
    ifstream ifs1(_conf._path["enIdx"]);
    while(getline(ifs1, line))
    {
        istringstream iss1(line);
        while(iss1 >> word)
        {
            while(iss1 >> num)
            {
                _enIdx[word].insert(num);
            }
        }
    }
    ifs1.close();

    ifstream ifs2(_conf._path["cnIdx"]);
    while(getline(ifs2, line))
    {
        istringstream iss2(line);
        while(iss2 >> word)
        {
            while(iss2 >> num)
            {
                _cnIdx[word].insert(num);
            }
        }
    }
    ifs2.close();
}
