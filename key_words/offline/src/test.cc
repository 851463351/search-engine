#include "../../include/dictionary.hpp"
#include "../../include/SplitToolCppJieba.hpp"

int main()
{
    /* Configuration conf("./../../conf/dic_conf"); */
    /* map<string, string> path = conf.getConfigMap(); */
    /* cout << path["enConfig"] << endl; */

    /* cout << "1" << endl; */

    /* DictProducer dic(path["enConfig"]); */
    /* dic.showFiles(); */
    /* cout << endl; */
    /* dic.buildEnDict(path["enStop"]); */
    /* dic.storeDict("./../../conf/enDic"); */
    /* dic.buildEnIndex(); */
    /* dic.storeIdx("./../../conf/enIdx"); */

    Configuration conf("./../../conf/dic_conf");
    map<string, string> path = conf.getConfigMap();
    cout << path["cnConfig"] << endl;


    DictProducer dic(path["cnConfig"], SplitToolCppJieba::getInstance());
    dic.showFiles();
    cout << endl;
    dic.buildCnDict();
    /* dic.showDict(); */
    dic.buildCnIndex();
    /* dic.showIndex(); */
    dic.storeDict("./../../conf/cnDic");
    dic.storeIdx("./../../conf/cnIdx");
    

    return 0;
}

