#include "../include/DirScanner.h"
#include "../include/configuration.h"
#include "../include/FileProcessor.h"
#include "../include/PageLib.h"
#include "../include/PageLibPreprocessor.h"
#include "../include/WebPage.h"

int main(int argc,char *argv[])
{

    Configuration conf("../conf/path.conf");
#if 1 
    DirScanner dir(conf);
    dir();
    cout<<"dir() finished"<<endl;
    
    FileProcessor file;
    PageLib page(conf,dir,file);
    page.create();
    cout<<"page.create() finished"<<endl;
    page.store();
    cout<<"page.store() finished"<<endl;
#endif
#if 1
    PageLibPreprocessor pp(conf);
    pp.doProcess();
#endif
    return 0;
}

