#include "../../include/dictionary.hpp"
#include "../../include/SplitTool.h"
#include <dirent.h>
#include <sstream>
#include <algorithm>

DictProducer::DictProducer(const string &dir)
: _dir(dir)
, _files()
, _dict()
, _index()
{
    getFiles();
}

DictProducer::DictProducer(const string &dir, SplitTool *splitTool)
: _dir(dir)
, _files()
, _dict()
, _index()
, _splitTool(splitTool)
{
    getFiles();
}

void DictProducer::getFiles()
{
    DIR *pdir = opendir(_dir.c_str());
    struct dirent *pdirent;
    while((pdirent = readdir(pdir)))
    {
        if(strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
            continue;
        _files.push_back(_dir + string("/") + string(pdirent->d_name));
    }
    closedir(pdir);
}

void DictProducer::buildEnDict(const string &stopWordsPath)
{
    string line;
    string word;
    for(auto &file : _files)
    {
        ifstream ifs(file);
        if(!ifs)
        {
            cerr << "ifs open " << file << " error!" << endl;
            return;
        }

        while(getline(ifs, line))
        {
            //标点符号转换为空格
            for(size_t i = 0; i < line.size(); ++i)
            {
                if(!isalpha(line[i]) && line[i] != ' ')
                    line[i] = ' ';
                else if(isupper(line[i]))
                    line[i] += 32;
            }

            istringstream iss(line);
            while(iss >> word)
            {
                std::pair<map<string, int>::iterator, bool> ret = _dict.insert(std::pair<string, int>(word, 1));
                if(!ret.second)
                {
                    ++_dict[word];
                }
            }
        }
        ifs.close();
    }

    //删除停用词
    ifstream ifs(stopWordsPath);
    if(!ifs)
    {
        cerr << "ifs open error!" << endl;
        return;
    }
    while(ifs >> word)
    {
        auto it = _dict.find(word);
        if(it != _dict.end())
        {
            _dict.erase(it);
        }
    }
    ifs.close();
}

void DictProducer::storeDict(const char *filepath)
{
    ofstream ofs(filepath);
    for(auto &elem : _dict)
    {
        ofs << elem.first << " " << elem.second << endl;
    }
    
    ofs.close();
}

void DictProducer::buildEnIndex()
{
    int idx = 0;
    for(auto &elem : _dict)
    {
        for(size_t i = 0; i < elem.first.size(); ++i)
        {
            string tmp;
            tmp.push_back(elem.first[i]);
            _index[tmp].insert(idx);
        }
        ++idx;
    }
}

void DictProducer::storeIdx(const char *filepath)
{
    ofstream ofs(filepath);
    for(auto &elem : _index)
    {
        ofs << elem.first << " ";
        for(auto &num : elem.second)
        {
            ofs << num << " ";
        }
        ofs << endl;
    }
    ofs.close();
}

void DictProducer::buildCnDict()
{
    for (auto& filename : _files) {
        ifstream ifs(filename);
        if (!ifs) {
            cerr << filename << "open failed" << endl;
        }
        string sentence;
        vector<string> words;
        while (getline(ifs, sentence)) {
            words = _splitTool->cut(sentence);
            for (auto& word : words) {//用来删除非汉字
                word.erase(
                           remove_if ( word.begin(), word.end(), static_cast<int(*)(int)>(&ispunct)  ),
                           word.end()
                          );
                word.erase(
                           remove_if ( word.begin(), word.end(), static_cast<int(*)(int)>(&isspace)  ),
                           word.end()
                          );
                word.erase(
                           remove_if ( word.begin(), word.end(), static_cast<int(*)(int)>(&isgraph)  ),
                           word.end()
                          );
                word.erase(
                           remove_if ( word.begin(), word.end(), static_cast<int(*)(int)>(&isalnum)  ),
                           word.end()
                          );
                if(word != string())
                    ++_dict[word];
            }
        }
        ifs.close();
    }
}

void DictProducer::showDict() const
{
    for(auto &elem : _dict)
    {
        cout << elem.first << " " << elem.second << endl;
    }
}

//中文分字算法
void splitWord(const string &word, vector<string> &characters)
{
    int num = word.size();
    int i = 0;
    string subWord;
    while(i < num)
    {
        int size = 1;
        if(word[i] & 0x80)//0x80 三字节代表一个中文汉字
        {
            char temp = word[i];
            temp <<= 1;
            do{
                temp <<= 1;
                ++size;

            }while(temp & 0x80);
        }
        subWord = word.substr(i, size);
        characters.push_back(subWord);
        i += size;
    }
}

void DictProducer::buildCnIndex()
{
    vector<string> characters;//用来存储单词分开后的汉字
    size_t num = 0;
    for(auto &elem : _dict)
    {   
        characters.clear();
        splitWord(elem.first, characters);
        for(auto ch : characters)
        {
            _index[ch].insert(num);
        }
        ++num;
    }
}

void DictProducer::showIndex() const
{
    for(auto &elem : _index)
    {
        cout << elem.first << "->";
        for(auto &num : elem.second)
        {
            cout << num << " ";
        }
        cout << endl;
    }
}
