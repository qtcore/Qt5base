#include <QtWidgets>
#include <QtNetwork>

#include <stdlib.h>
#include "dialog.h"
#include "fortuneserver.h"

Dialog::Dialog(QWidget *parent)
    : QWidget (parent)
{
    statusLabel = new QLabel;
    statusLabel->setWordWrap(true);
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

    if(!server.listen()) {
        QMessageBox::critical(this, tr("Threaded Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server.errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();

    //使用第一个非本地主机IPV4地址
    for(int i=0; i<ipAddressList.size(); ++i) {
        if(ipAddressList.at(i) != QHostAddress::LocalHost &&
                ipAddressList.at(i).toIPv4Address()) {
                ipAddress = ipAddressList.at(i).toString();
                break;
    }

                //如果我们没有找到，则使用本地主机的IPV4地址
                if(ipAddress.isEmpty())
                ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
        statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                                "Run the Fortune Client example now.")
                             .arg(ipAddress).arg(server.serverPort()));
        connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

           QHBoxLayout *buttonLayout = new QHBoxLayout;
            buttonLayout->addStretch(1);
            buttonLayout->addWidget(quitButton);
            buttonLayout->addStretch(1);

            QVBoxLayout *mainLayout = new QVBoxLayout;
            mainLayout->addWidget(statusLabel);
            mainLayout->addLayout(buttonLayout);
            setLayout(mainLayout);
            setWindowTitle(tr("Threaded Fortune Server"));
    }
}
