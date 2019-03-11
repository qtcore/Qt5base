#include "fortuneserver.h"
#include <QApplication>
#include <stdlib.h>
#include <QtCore>
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog dialog;
    dialog.show();

    return a.exec();
}
