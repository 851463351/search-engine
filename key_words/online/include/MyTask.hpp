#pragma once
#include "TcpServer.hpp"
#include "Dict.hpp"
#include <queue>
#include <vector>
using namespace std;

struct Result
{
    Result(const string &word, int freq, int dist)
    : _word(word)
    , _freq(freq)
    , _dist(dist)
    {}

    string _word;
    int _freq;
    int _dist;
};

struct MyCompare
{
    bool operator()(const Result &lhs, const Result &rhs) const
    {
        if(lhs._dist != rhs._dist)
        {
            return lhs._dist > rhs._dist;
        }
        else
        {
            if(lhs._freq != rhs._freq)
            {
                return lhs._freq > rhs._freq;
            }
            else
            {
                return lhs._word > rhs._word;
            }
        }
    }
};

class MyTask
{
public:
    MyTask(const string &queryWord, const wd::TcpConnectionPtr &conn, Mydict *dict); 
    ~MyTask() {}

    int distance(const string & rhs);  //计算中英文最小编辑距离
    void execute();  //执行查询
    void statistic(set<int> & iset);  //计算每一个关联词与查询词的距离，并插入优先级队列

private:
    size_t nBytesCode(const char ch);
    size_t length(const string &str);
    void parseWord(vector<string> &chars);  //按单个字符分词，每个存入vector
    string packetJson(); //将优先级队列前几个元素打包成jason包
    void packetMessage(string &result); //打包jaon为消息协议再交给IO线程发送

private:
    string _queryWord;
    Mydict *_dict; 
    wd::TcpConnectionPtr _conn; 
    priority_queue<Result, vector<Result>, MyCompare> _resultQue;
    short _flag;
};

