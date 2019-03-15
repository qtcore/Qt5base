#include <QCoreApplication>
#include <QStringList>
#include "downloadmanager.h"
#include <cstdio>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

     QStringList arguments = a.arguments();
     arguments.takeFirst(); //删除第一个参数，即程序的名称

     if(arguments.isEmpty()) {
         printf("Qt Download example\n"
                "Usage: downloadmanager url1 [url2... urlN]\n"
                               "\n"
                               "Downloads the URLs passed in the command-line to the local directory\n"
                               "If the target file already exists, a .0, .1, .2, etc. is appended to\n"
                               "differentiate.\n");
                        return 0;


     }

     DownloadManager manager;
     manager.append(arguments);

     QObject::connect(&manager, SIGNAL(finished()), &a, SLOT(quit()));

    return a.exec();
}
