#pragma once
#include <json/json.h>
#include <iostream>
#include <vector>
#include <string>
using std::string;
using std::vector;
class json
{
public:
    json() {}
    ~json() {}
    string pakage(const string& searchWord);
    std::vector<string> unpakage(const string &lhs);
    string pakage(const vector<string> & candidate);
private:

};
string json::pakage(const string & searchWord){
    Json::Value arrayObj;
    Json::Value item;
    item["candidate"]=searchWord;
    arrayObj.append(item);
    Json::FastWriter writer;
    string result=writer.write(arrayObj);

    return result;
}

string json::pakage(const vector<string> & candidate){
    Json::Value arrayObj;

    for(size_t i=0;i<candidate.size();++i)
    {
        Json::Value item;
        item["candidate"]=candidate[i];
        arrayObj.append(item);

    }

    Json::FastWriter writer;

    string result=writer.write(arrayObj);

    return result;
}
std::vector<string> json::unpakage(const string &lhs){
    Json::Reader reader;
    Json::Value root;
    vector<string> res;
    if(reader.parse(lhs,root))
    {
        if(root.isArray())
        {
            int nArrarySize=root.size();

            for(int idx=0;idx<nArrarySize;++idx)
            {
                string word=root[idx]["candidate"].asString();
                /* std::cout<<"string="<<word<<std::endl; */
                res.push_back(word);
            }
        }
    }
    return res; 
}
