#include "receiver.h"
#include <QtWidgets>
#include <QtNetwork/QtNetwork>


Receiver::Receiver(QWidget *parent)
    : QWidget(parent)
{
    statusLabel = new QLabel(tr("Listening for broadcasted messages"));
    statusLabel->setWordWrap(true);

    quitButton = new QPushButton(tr("&Quit"));

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress);

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));

    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Broadcast Receiver"));

}


void Receiver::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams()){

        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        statusLabel->setText(tr("Received datagram: \"%1\"")
                                .arg(datagram.data()));

    }


}
