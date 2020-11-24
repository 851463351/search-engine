#include "../include/PageLibPreprocessor.h"
#include "../include/configuration.h"
#include "../include/WebPage.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <queue>

using std::cout;
using std::endl;
using std::cin;
using std::istringstream;
using std::ifstream;
using std::ofstream;
using std::queue;
PageLibPreprocessor::PageLibPreprocessor(Configuration &conf)
:_conf(conf)
{}

void PageLibPreprocessor::doProcess()
{
    /* cout<<"111"<<endl; */
    readInfoFromFile();
    cutRedundantPages();
    /* cout<<"222"<<endl; */
    buildInvertIndexTable(); 
    storeOnDisk();
}

void PageLibPreprocessor::readInfoFromFile()
{
    vector<string> vecfiles;
    //网页库和偏移库的地址
    string doc_path,offset_path;
    doc_path=_conf.getConfigMap()["ripepage_path"];
    offset_path=_conf.getConfigMap()["offset_path"];
    ifstream in_page(doc_path,std::ios::binary | std::ios::ate);
    if(!in_page.good())
    {
        /* cout<<"333"<<endl; */
        return; 
    }
    ifstream in_offset(offset_path);
    if(!in_offset.good())
    {
        /* cout<<"444"<<endl; */
        return;
    }
    int id=0;
    int start=0;
    int len=0;
    string line;//获取偏移库的每一行
    while(getline(in_offset,line))
    {
        istringstream iss(line);
        iss>>id;
        iss>>start;
        iss>>len;
        string text(len,0);//保存网页
        /* _offsetLib[id]=std::make_pair(start,len); */
        in_page.seekg(start,std::ios::beg);
        in_page.read(&text[0],len);
        /* cout<<text<<endl; */
        vecfiles.push_back(text);
    }
    /* for(auto &i:_vecfiles) */
    /* { */
    /*     cout<<i<<endl<<endl<<endl; */
    /* } */
    for(size_t i=0;i!=vecfiles.size();++i)
    {
        WebPage w;
        w.processDoc(vecfiles[i]);
        w.createDictMap(_conf);
        w.calcTopk(10);
        _pageLib.push_back(w);
    }
}

void PageLibPreprocessor::cutRedundantPages() //网页库去重
{
    for(auto l_iter=_pageLib.begin();l_iter!=_pageLib.end();++l_iter)
    {
        auto tempiter=l_iter;
        for(auto r_iter=++tempiter;r_iter!=_pageLib.end();)
        {
            if((*l_iter)==(*r_iter))
            {
                r_iter=_pageLib.erase(r_iter);
            }
            else
            {
                ++r_iter;
            }
        }
    }

}

void PageLibPreprocessor::storeOnDisk()
{
    int start=0;
    for(auto & page:_pageLib)
    {
        int len=page.getDoc().size();
        _offsetLib[page.getDocId()]=std::make_pair(start,len);
        start+=len;
    }

    string off_path;
    off_path=_conf.getConfigMap()["reoffset_path"];
    ofstream out_off(off_path);
    for(auto &it:_offsetLib)
    {
        out_off<<it.first<<" "<<it.second.first<<" "<<it.second.second<<endl;
    }
    string page_path;
    page_path=_conf.getConfigMap()["repage_path"];
    ofstream out_page(page_path);
    /* cout<<"page_path.size()= "<<_pageLib.size()<<endl; */
    for(auto &page:_pageLib)
    {
        /* cout<<"page_path.size()= "<<page_path.size()<<endl; */
        out_page<<page.getDoc();
    }

    string invertIndex_path=_conf.getConfigMap()["invertIndex_path"];
    ofstream out_invertIndex(invertIndex_path);
    if(!out_invertIndex.good())
    {
        cout<<"the ofstream "<<invertIndex_path<<" open failed"<<endl;
        return;
    }
    for(auto & elem :_invertIndexTable)
    {
        out_invertIndex<<elem.first<<" ";
        for(auto & j:elem.second)
        {
            out_invertIndex<<j.first<<" "<<j.second<<" ";
        }
        out_invertIndex<<endl;
    }
}


void PageLibPreprocessor::buildInvertIndexTable()  //建立倒排索引表
{
    cout<<"111"<<endl;
    int N=_pageLib.size();
    
    map<int,unordered_map<string,double>>  pageWordsWeight;
    /* vector<map<string,int>> pagesWords; */
    /* queue<vector<double>> pagesW; */
    
    /* for(auto &page: _pageLib) */
    /* { */
    /*     pagesWords.push_back(page.getWordsMap()); */
    /* } */
    cout<<"222"<<endl;

    for(auto & page:_pageLib)
    {
        int idDoc=page.getDocId();
        unordered_map<string,int> wordsMap=page.getWordsMap();
        for(auto  &elem: wordsMap)
        {
            int TF=elem.second;
            int DF=0;
            for(auto & web:_pageLib)
            {
                unordered_map<string,int> reWordsMap=web.getWordsMap();
                if(reWordsMap.count(elem.first)==1)
                {
                    ++DF;
                }

            }
            double IDF=log2(N/(DF+1));
            double w=TF*IDF;
            /* W.push_back(w); */
            pageWordsWeight[idDoc].insert(std::make_pair(elem.first,w));
        }
        //一篇文章每一个关键词的w计算到了
        /* pagesW.push(W); */
    }

    for(auto & elem:pageWordsWeight)
    {
        for(auto wordsWeight : elem.second)
        {
            double tempW=0;
            for(auto reWordsWeight:elem.second)
            {
                tempW+=reWordsWeight.second*reWordsWeight.second;
            }
            double weight=wordsWeight.second/sqrt(tempW);
            _invertIndexTable[wordsWeight.first].push_back(std::make_pair(elem.first,weight));
            
        }
    }

    cout<<"333"<<endl;
    /* for(auto &page:_pageLib) */
    /* { */
    /*     map<string,int> pageMap=page.getWordsMap(); */
    /*     for(auto & elem: pageMap) */
    /*     { */
    /*         cout<<"333.1"<<endl; */
    /*         int TF=elem.second; */
    /*         int DF=0; */
    /*         for(auto &wordsMap:pagesWords) */
    /*         { */
    /*             if(wordsMap.count(elem.first)==1) */
    /*             { */
    /*                 ++DF; */
    /*             } */

    /*         } */
    /*         cout<<"333.2"<<endl; */
    /*         double IDF=log2(N/(DF+1)); */
    /*         double w=TF*IDF; */
            
    /*         vector<double> wplus=pagesW.front(); */
            
    /*         cout<<"333.4"<<endl; */

    /*         double tempW=0; */
    /*         for(auto & i:wplus) */
    /*         { */
    /*             tempW+=(i*i); */
    /*         } */
    /*         cout<<"333.5"<<endl; */
    /*         double weight=w/sqrt(tempW); */
    /*         cout<<"weight"<<endl; */
    /*         _invertIndexTable[elem.first].push_back(std::make_pair(page.getDocId(),weight)); */

/*         } */
/*         pagesW.pop(); */
        /* cout<<"444"<<endl; */
    /* } */
}
