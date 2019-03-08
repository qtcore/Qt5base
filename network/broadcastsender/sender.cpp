#include "sender.h"
#include <QtWidgets>
#include <QtNetwork/QtNetwork>
#include <QtCore>

Sender::Sender(QWidget *parent)
    : QWidget(parent)
{
    statusLabel = new QLabel(tr("Ready to broadcast datagrams on port 45454"));
    statusLabel->setWordWrap(true);

    startButton = new QPushButton(tr("&Start"));
    auto quitButton = new QPushButton(tr("&Quit"));
    stopButton = new QPushButton(tr("&Stop"));//添加停止发送广播按钮

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton,QDialogButtonBox::RejectRole);

    buttonBox->addButton(stopButton, QDialogButtonBox::ActionRole);

    udpSocket = new QUdpSocket(this);

    connect(startButton, &QPushButton::clicked, this, &Sender::startBroadcasting);
    connect(quitButton, &QPushButton::clicked, this, &Sender::close);
    connect(&timer, &QTimer::timeout, this, &Sender::broadcastDatagram);
    connect(stopButton, &QPushButton::clicked, this, &Sender::stopBroadcasting);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Broadcast Sender"));

}


void Sender::startBroadcasting()
{
    startButton->setEnabled(false);
    timer.start(1000);
    stopButton->setEnabled(true);


}

//停止广播
void Sender::stopBroadcasting()
{
    stopButton->setEnabled(false);
    startButton->setEnabled(true);
    timer.stop();
    statusLabel->setText(tr("Ready to broadcast again"));
   // udpSocket->close();

}

void Sender::broadcastDatagram()
{
  //  udpSocket->abort();
    statusLabel->setText(tr("Now broadcasting datagram %1").arg(messageNo));

    QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo);
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 45454);

    ++messageNo;
}


