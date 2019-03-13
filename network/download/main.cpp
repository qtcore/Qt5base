#include <QCoreApplication>

#include <QtCore>
#include <QtNetwork/QtNetwork>
#include <cstdio>
#include "downloadmanager.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DownloadManager manager;
    QTimer::singleShot(0, &manager, SLOT(execute()));

    return a.exec();
}

//#include "main.moc"
