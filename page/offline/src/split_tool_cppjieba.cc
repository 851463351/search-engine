#include "../include/split_tool_cppjieba.h"
#include "../lib/cppjieba/include/Jieba.hpp"
#include <locale>
#include <codecvt>

const char* const DICT_PATH = "../lib/cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../lib/cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../lib/cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "../lib/cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../lib/cppjieba/dict/stop_words.utf8";

using namespace cppjieba;

Jieba SplitToolCppJieba::_jieba(DICT_PATH, HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH);

void handleUtf8String(string &s)
{
    locale loc("en_US.UTF-8");
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    wstring ws=conv.from_bytes(s);
    wstring nws;
    for(wchar_t &ch:ws)
    {
        if(!ispunct(ch,loc)&&!iswspace(ch)&&!iswalnum(ch))
        {
            nws.push_back(ch);
        }
    }
    s=conv.to_bytes(nws);
    /* for(auto i=s.begin();i!=s.end();) */
    /* { */
    /*     if(iswalnum(*i)) */
    /*     { */
    /*         i=s.erase(i); */
    /*     } */
    /*     else */
    /*     { */
    /*         ++i; */
    /*     } */
    /* } */
}

vector<string> SplitToolCppJieba::cut(const string &sentence)
{
    /* cout<<"jieba 111"<<endl; */
    vector<string> words;
    string s;
    s=sentence;
    vector<KeywordExtractor::Word> keywordres;
    int topk=1000000;
    _jieba.extractor.Extract(s,keywordres,topk);
    /* cout<<"jieba 222"<<endl; */
    /* handleUtf8String(s); */
    /* cout<<s<<endl; */
    /* jieba.CutForSearch(s,words); */
    for(size_t i=0;i<keywordres.size();++i)
    {
        words.push_back(keywordres[i].word);
    }
    /* cout<<"jieba 333"<<endl; */
    //提取关键词之后将数字与字母从字典中去除
    for(auto i=words.begin();i!=words.end();)
    {
        if(isalnum((*i)[0]))
        {
            i=words.erase(i);
        }
        else
        {
            ++i;
        }
    }
    /* cout<<"jieba 444"<<endl; */
    /* for(auto &i:words) */
    /* { */
    /*     cout<<i<<endl; */
    /* } */
    return words;
}
