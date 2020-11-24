#include "../include/WebPageQuery.h"
#include <json/json.h>
#include <string.h>

WebPageQuery::WebPageQuery(const string & queryPhrase,const TcpConnectionPtr & pConn,Configuration & conf)
    :_queryPhrase(queryPhrase)
    ,_pConn(pConn)
    ,_conf(conf)
{}

//加载库文件
void WebPageQuery::loadLibrary()
{
    loadPageLib(); //加载网页库
    loadInvertIndexTable(); //加载倒排索引表
}

void WebPageQuery::loadPageLib()
{
    vector<string> vecfiles;

    string page_path=_conf.getConfigMap()["repage_path"];
    string offset_path=_conf.getConfigMap()["reoffset_path"];

    ifstream in_page(page_path,std::ios::binary | std::ios::ate);
    if(!in_page.good())
    {
        cout<<"the ifstream "<<page_path<< "open failed "<<endl;
        return; 
    }

    ifstream in_offset(offset_path);
    if(!in_offset.good())
    {
        cout<<"the ifstream "<<offset_path<<" open failed "<<endl;
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
        string text(len,0);  //每一篇文章的内容
        _offsetLib[id]=std::make_pair(start,len);
        in_page.seekg(start,std::ios::beg);
        in_page.read(&text[0],len);
        /* cout<<text<<endl; */
        vecfiles.push_back(text);
    }

    /* for(auto &i:vecfiles) */
    /* { */
    /*     cout<<i<<endl<<endl<<endl; */
    /* } */

    for(size_t i=0;i!=vecfiles.size();++i)
    {
        WebPage w;
        w.processDoc(vecfiles[i]);
        w.createDictMap(_conf);
        _pageLib[w.getDocId()]=w;
    }
}

void WebPageQuery::loadInvertIndexTable()
{
    string invertIndex_path=_conf.getConfigMap()["invertIndex_path"];
    ifstream ifs(invertIndex_path);
    if(!ifs.good())
    {
        cout<<"the ifstream "<<invertIndex_path<<" oen failed "<<endl;
        return;
    }

    string line;
    while(getline(ifs,line))
    {
        int docId;
        double weight;
        string word;

        istringstream iss(line);
        iss>>word;
        while(iss>>docId && iss>>weight)
        {
            _invertIndexTable[word].insert(std::make_pair(docId,weight));
        }
    }
}

vector<string> WebPageQuery::wordSegmentation(const string & queryPhrase)
{
    return SplitToolCppJieba::cut(queryPhrase);
}

void WebPageQuery::createStopWordsSets(unordered_set<string> & stopWords)
{
    string stopWords_path=_conf.getConfigMap()["stopWords_path"];
    ifstream ifs(stopWords_path);
    if(!ifs.good())
    {
        cout<<"the ifstream "<<stopWords_path<<" open failed"<<endl;
        return;
    }
    string word;
    while(ifs>>word)
    {
        stopWords.insert(word);
    }

}

void WebPageQuery::getsQueryWordsWeightVector(vector<string> &queryWords,vector<double> & base)
{

    unordered_set<string> stopWords;
    createStopWordsSets(stopWords);

    //1.计算查询语句中每个单词的词频
    unordered_map<string,int> QueryWordsFreq;
    for(auto & word:queryWords)
    {
        if(stopWords.count(word)==0)
        {
            ++QueryWordsFreq[word];
        }
    }
    //2.计算每一个单词在网页库中的w权重

    int N=_pageLib.size();

    vector<double> weight;
    for(auto & word: QueryWordsFreq)
    {
        int TF=word.second;
        int DF=0;
        for(auto web:_pageLib)
        {
            unordered_map<string,int> wordsMap=web.second.getWordsMap();
            if(wordsMap.count(word.first)==1)
            {
                ++DF;
            }
        }
        double IDF=log2(N/DF+1);
        double w=TF*IDF;
        weight.push_back(w);
    }

    //3.计算w的平方和
    double tempSum=0;
    for(auto elem:weight)
    {
        tempSum+=elem*elem;
    }
    //4.计算每一单词的w‘，即base
    for(auto elem :weight)
    {
        base.push_back(elem/sqrt(tempSum));
    }

}

bool WebPageQuery::executeQuery(const vector<string> &queryWords,unordered_map<int,vector<double>> &resultVec)
{
    if(queryWords.empty())
    {
        return false;
    }
    for(auto & word:queryWords)
    {
        auto iter=_invertIndexTable.find(word);
        if(iter!=_invertIndexTable.end())
        {
            set<std::pair<int,double>> sets=iter->second;
            for(auto &elem:sets)
            {
                resultVec[elem.first].push_back(elem.second);
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}

void WebPageQuery::calcSort(const vector<double> &base,unordered_map<int,vector<double>> & resultVec,vector<int> &docIdVec)
{
    vector<std::pair<int,double>>  docSimilarity;
    //1.计算base的模长
    double baseLenth=0;
    for(auto value:base)
    {
        baseLenth+=value*value;
        /* cout<<"baseLenth:="<<baseLenth<<endl; */
    }
    baseLenth=sqrt(baseLenth);
    /* cout<<"baseLenth="<<baseLenth<<endl; */

    //2.计算每一篇文章与base的相似度
    for(auto elem:resultVec)
    {
        int index=0;
        double multiply=0;
        double yLenth=0;
        for(auto  value: elem.second)
        {
            multiply+=base[index]*value;
            yLenth+=value*value;
            ++index;
        }
        yLenth=sqrt(yLenth);
        double similarity=multiply/(baseLenth*yLenth);
        docSimilarity.push_back(std::make_pair(elem.first,similarity));
    }

    /* for(auto elem:docSimilarity) */
    /* { */
    /*     cout<<"docSimilarity.id:="<<elem.first<<" "<<elem.second<<endl; */
    /* } */

    //3.排序
    std::sort(docSimilarity.begin(),docSimilarity.end(),
              [](const std::pair<int,double> & lhs,const std::pair<int,double> &rhs){return lhs.second>rhs.second;});

    for(auto elem:docSimilarity)
    {
        docIdVec.push_back(elem.first);
    }
}

string WebPageQuery::createJson(vector<int> & docIdVec,const vector<string> & queryWords)
{
    Json::Value arrayObj;
    Json::FastWriter writer;
    for(auto docid:docIdVec)
    {
        WebPage web=_pageLib[docid];
        string title=web.getTitle();
        string content=web.getContent();
        string summary;
        for(auto word:queryWords)
        {
            auto pos=content.find(word,0);
            if(pos!=std::string::npos)
            {
                summary=content.substr(pos,90)+"..."+"\n";
                break;
            }
        }
        if(summary.empty())
        {
            summary=content.substr(0,90)+"..."+"\n";
        }
        //打包成一个字符串
        arrayObj.append(packJson(docid,title,summary));
    }
    string result=writer.write(arrayObj);
    /* cout<<"createJson::result="<<result<<endl; */
    return result;
}


Json::Value WebPageQuery::packJson(int docid, const string & title,const string & summary)
{
    Json::Value arrayObj;
    Json::Value item;
    item=std::to_string(docid);
    arrayObj.append(item);
    item=title;
    arrayObj.append(item);
    item=summary;
    arrayObj.append(item);
    
    return arrayObj;
}

string WebPageQuery::returnNoAnswer()
{
    Json::Value obj;
    Json::Value noAnswer;
    noAnswer="the queryPhrase is non-existent";
    obj.append(noAnswer);

    Json::FastWriter writer;
    string result=writer.write(obj);
    return result;
}

void WebPageQuery::packMessage(const string &result)
{
    Message mgs;
    mgs._id=200;
    mgs._len=result.size();
    strcpy(mgs._content,result.c_str());
    _pConn->sendInloop(mgs);
}

void WebPageQuery::doQuery()
{
    string result;
    //1.加载库文件
    /* cout<<"111"<<endl; */
    loadLibrary();

    //2.对查询短语进行分词
    /* cout<<"222"<<endl; */
    vector<string> queryWords=wordSegmentation(_queryPhrase);
    /* cout<<"queryWords.size()="<<queryWords.size()<<endl; */
    /* cout<<"queryWords:words: "; */
    /* for(auto word:queryWords) */
    /* { */
    /*     cout<<word<<" "; */
    /* } */
    /* cout<<endl; */

    //3.执行查询
    /* cout<<"333"<<endl; */
    unordered_map<int,vector<double>> resultVec;
    bool isAnswer=executeQuery(queryWords,resultVec);
    if(isAnswer==false)
    {
        /* cout<<"444.1"<<endl; */
        result=returnNoAnswer();
        /* cout<<"returnNoAnswer::result="<<result<<endl; */
    }
    else
    {
        /* cout<<"resultVec:: "; */
        /* for(auto elem:resultVec) */
        /* { */
        /*     cout<<"docid:"<<elem.first<<" "<<"weights: "; */
        /*     for(auto num:elem.second) */
        /*     { */
        /*         cout<<num<<" "; */
        /*     } */
        /*     cout<<endl; */
        /* } */
        /* cout<<endl; */

        /* cout<<"444"<<endl; */
        //4.计算base向量
        vector<double> base;
        getsQueryWordsWeightVector(queryWords,base);

        /* cout<<"base::"; */
        /* for(auto num: base) */
        /* { */
        /*     cout<<num<<" "; */
        /* } */
        /* cout<<endl; */
        //5.计算余弦值并排序
        /* cout<<"555"<<endl; */
        vector<int> docIdVec;
        calcSort(base,resultVec,docIdVec);
        /* cout<<"docIdVec::"; */
        /* for(auto id:docIdVec) */
        /* { */
        /*     cout<<id<<" "; */
        /* } */
        /* cout<<endl; */
        //6.返回要发送的文章内容
        /* cout<<"666"<<endl; */
        result=createJson(docIdVec,queryWords);
        /* cout<<"createJson::result="<<result<<endl; */
    }
    //7.Message协议发送
    /* cout<<"777"<<endl; */
    packMessage(result);
    /* cout<<"888"<<endl; */
}
