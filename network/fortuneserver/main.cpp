#include "server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationDisplayName(Server::tr("Fortune Server"));
    Server w;
    w.show();

    return a.exec();
}
