#include "../include/WebPage.h"
#include <regex>
#include <algorithm>


void WebPage::processDoc(const string &doc)
{
    _doc=doc;
        
    string cutStr=std::regex_replace(doc,std::regex("<[^>]*>")," ");

   int pos1=cutStr.find_first_of(" ",2);
   _docId=stoi(cutStr.substr(2,pos1-2));
   int pos2=cutStr.find_first_of(" ",pos1+2);
   _docUrl=cutStr.substr(pos1+2,pos2-pos1-2);
   int pos3=cutStr.find_first_of(" ",pos2+2);
   _docTitle=cutStr.substr(pos2+2,pos3-pos2-2);
   int pos4=cutStr.find_first_of(" ",pos3+2);
   _docContent=cutStr.substr(pos3+2,pos4-pos3-2);

    /* cout<<"_docId="<<_docId<<endl; */
    /* cout<<"_docUrl="<<_docUrl<<endl; */
    /* cout<<"_docTitle="<<_docTitle<<endl; */
    /* cout<<"_docContent="<<_docContent<<endl; */

}
void WebPage::createDictMap(Configuration & conf)
{

    ifstream ifs(conf.getConfigMap()["stopWord_path"]);
    if(!ifs.good())
    {
        cout<<"the ifstream "<<conf.getConfigMap()["stopWord_path"]<<" open failed"<<endl;
        return;
    }
    string word;
    unordered_set<string> stopWordSets;
    while(ifs>>word)
    {
        stopWordSets.insert(word);   
    }
    string tempstr=_docTitle+" "+_docContent;
    vector<string> contentWords=SplitToolCppJieba::cut(tempstr);
    for(auto & elem :contentWords)
    {
        if(stopWordSets.count(elem)==0)
        {
            ++_wordsMap[elem];        
        }
    }
}

void WebPage::calcTopk(size_t k)
{
    if(_wordsMap.size()<=k)
    {
        for(auto & elem: _wordsMap)
        {
            _topWords.push_back(elem.first);

        }
    }
    else
    {
        priority_queue<wordsFreq>  wordsfreq;

        for(auto elem :_wordsMap)
        {
            wordsfreq.push(wordsFreq(elem.first,elem.second));
        }
        for(size_t idx=0;idx!=k;++idx)
        {
            _topWords.push_back(wordsfreq.top()._word);
            wordsfreq.pop();
        }
        
    }
    cout<<"_topWords.size()="<<_topWords.size()<<endl;
}


bool operator==(const WebPage &lhs,const WebPage &rhs)
{
    vector<string> l_topWords=lhs.getTopWords();
    vector<string> r_topWords=rhs.getTopWords();
    cout<<"l_topWords.size()="<<l_topWords.size()<<endl;
    cout<<"r_topWords.size()="<<r_topWords.size()<<endl;

    vector<string> intersectionWords;
    std::set_intersection(l_topWords.begin(),l_topWords.end(),r_topWords.begin(),r_topWords.end(),std::back_inserter(intersectionWords));
    
    int sz=intersectionWords.size();
    double coinRate=sz*2.0/(l_topWords.size()+r_topWords.size());
    cout<<"coinRate="<<coinRate<<endl<<endl<<endl;

    if(coinRate>0.80)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

bool operator<(const wordsFreq &lhs,const wordsFreq &rhs)
{
    return lhs._freq<rhs._freq;
}




