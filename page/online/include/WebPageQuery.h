#ifndef _WENPAGEQUERY_H_
#define _WENPAGEQUERY_H_

#include "configuration.h"
#include "WebPage.h"
#include "Message.h"
#include "../src/Reactor/TcpConnection.h"
#include <json/json.h>


class WebPageQuery
{
public:
    WebPageQuery(const string & queryPhrase,const TcpConnectionPtr & pConn,Configuration & conf);
    void loadLibrary();  //加载库文件
    void doQuery(); //执行查询，返回结果
    void getsQueryWordsWeightVector(vector<string> & queryWords,vector<double> & base); //计算查询词的权重值
    bool executeQuery(const vector<string> & queryWords,unordered_map<int,vector<double>> &resultVec); //执行查询
    string createJson(vector<int> & docIdVec,const vector<string> &queryWords);
    string returnNoAnswer();
    void calcSort(const vector<double> & base,unordered_map<int,vector<double>> &resultVec,vector<int> &docIdVec); //计算余弦值并排序
private:
    void loadPageLib();
    void loadInvertIndexTable();
    vector<string> wordSegmentation(const string & queryPhrase);
    void  createStopWordsSets(unordered_set<string> & stopWords);
    Json::Value  packJson(int docid, const string & title,const string & summary);
    void packMessage(const string & result);

private:
    string _queryPhrase;
    TcpConnectionPtr _pConn;
    Configuration & _conf;
    unordered_map<int,WebPage> _pageLib;
    unordered_map<int,std::pair<int,int>> _offsetLib;
    unordered_map<string,set<std::pair<int,double>>> _invertIndexTable;
    

};

#endif
