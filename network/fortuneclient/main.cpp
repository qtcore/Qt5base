#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
     QApplication::setApplicationDisplayName(Client::tr("Fortune Client"));
    Client client;
    client.show();

    return a.exec();
}
