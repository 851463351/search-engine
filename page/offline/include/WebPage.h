#ifndef _WEBPAGE_H_
#define _WEBPAGE_H_

#include "../include/split_tool_cppjieba.h"
#include "../include/configuration.h"
#include <iostream>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <string>
#include <queue>

using std::unordered_set;
using std::priority_queue;
using std::set;
using std::unordered_map;

struct wordsFreq
{
    wordsFreq(const string & word,int freq)
        :_word(word)
        ,_freq(freq)
    {}
    string _word;
    int _freq;
    friend bool operator<(const wordsFreq &lhs,const wordsFreq &rhs);
};




class WebPage
{
public:
    friend bool operator==(const WebPage &lhs,const WebPage &rhs);
    
    void processDoc(const string &doc);  //处理格式化文档
    void createDictMap(Configuration & conf); //创建词典，存储文档所以关键词和词频
    void calcTopk(size_t k);  //获取词频最高的k个关键词
    
    int getDocId() {return _docId;}
    string getDoc()  {return _doc;}
    unordered_map<string,int> & getWordsMap()  { return _wordsMap; }
    const vector<string> & getTopWords() const  { return _topWords;}
private:

    string _doc; //存储格式化文档
    int _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;
    unordered_map<string,int> _wordsMap; //文章所有的关键词及词频
    vector<string> _topWords;  //每篇文章中频率最高的单词 
};

#endif
