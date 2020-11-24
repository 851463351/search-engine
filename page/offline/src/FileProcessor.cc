#include "../include/FileProcessor.h"
#include "../include/tinyxml2.h"
#include <regex>
#include <iostream>

using std::cout;
using std::endl;
using namespace tinyxml2;


void FileProcessor::parse_xml(const string &filepath,vector<string> &vecFilesfilesi,int &docid,int &start,map<int,pair<int,int>> &offsetlib)
{
    XMLDocument doc;
    //加载xml文件
    if(doc.LoadFile(filepath.c_str())!=0)
    {
        return;
    }
    XMLElement *root=doc.RootElement();
    XMLElement *channel=root->FirstChildElement("channel");
    XMLElement *item=channel->FirstChildElement("item");
    int pagelen=0;
    while(item)
    {
        string page;
        string title,content,tempContent;

        XMLElement *content_xml=item->FirstChildElement("content:encoded");
        if(content_xml==nullptr)
        {
            XMLElement *content_xml=item->FirstChildElement("description");
            tempContent=content_xml->GetText();
            /* cout<<"tempContent="<<tempContent<<endl; */
            /* cout<<"333"<<std::endl; */
        }
        else
        {
            tempContent=content_xml->GetText();
        }
        /* cout<<"tempContent="<<tempContent<<endl; */
        content=regex_replace(tempContent,std::regex("<[^>]*>"),"");
        /* cout<<"content="<<content<<endl; */
        
        XMLElement *title_xml=item->FirstChildElement("title");
        if(title_xml->FirstChild()==nullptr)
        {
            int sz=content.size();
            if(sz<10)
            {
                title=content+"...";
                /* cout<<"title= "<<title<<endl; */
            }
            else
            {
                title=content.substr(0,10)+"...";
                /* cout<<"title="<<title<<endl; */
            }
        }
        else
        {
            /* cout<<"888"<<endl; */
            title=title_xml->GetText();
            /* cout<<"999"<<endl; */
        }
        /* cout<<"title="<<title<<endl; */
        string tmppage;
        tmppage+="<doc><docid>"+std::to_string(docid)+"</docid><url>"+filepath
            +"</url><title>"+title+"</title><content>"+content+"</content></doc>";
        page=regex_replace(tmppage,std::regex(" "),"");
        /* page=regex_replace(tmppage,std::regex("\n"),""); */
        vecFilesfilesi.push_back(page);
        pagelen=page.size()+3;  //三个空行
        
        offsetlib[docid]=std::make_pair(start,pagelen);
        item=item->NextSiblingElement("item");
        start=start+pagelen;
        ++docid;
    }
}
