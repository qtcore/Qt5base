#include "dialog.h"

#include <QtNetwork/QtNetwork>
#include <QtWidgets>

static const int TotalBytes = 50 * 1024 * 1024;
static const int PayloadSize = 64 * 1024;


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    clientProgressBar = new QProgressBar;
    clientStatusLabel = new QLabel(tr("Client ready"));
    serverProgressBar = new QProgressBar;
    serverStatusLabel = new QLabel(tr("Server ready"));

    startButton = new QPushButton(tr("&Start"));
    quitButton = new QPushButton(tr("&Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton,QDialogButtonBox::RejectRole);

    connect(startButton, &QAbstractButton::clicked, this, &Dialog::start);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(&tcpServer, &QTcpServer::newConnection, this, &Dialog::acceptConnection);
    connect(&tcpClient, &QAbstractSocket::connected, this, &Dialog::startTransfer);
    connect(&tcpClient, &QIODevice::bytesWritten,
            this, &Dialog::updateClientProgress);
    connect(&tcpClient, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &Dialog::displayError);

    QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(clientProgressBar);
        mainLayout->addWidget(clientStatusLabel);
        mainLayout->addWidget(serverProgressBar);
        mainLayout->addWidget(serverStatusLabel);
        mainLayout->addStretch(1);
        mainLayout->addSpacing(10);
        mainLayout->addWidget(buttonBox);
        setLayout(mainLayout);

        setWindowTitle(tr("Loopback"));
}


void Dialog::start()
{
    startButton->setEnabled(false);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    bytesWritten = 0;
    bytesReceived = 0;

    while(!tcpServer.isListening() && !tcpServer.listen()) {
        QMessageBox::StandardButton ret = QMessageBox::critical(this,
                                              tr("Loopback"),
                                              tr("Unable to start the test: %1.")
                                                                .arg(tcpServer.errorString()),
                                                                QMessageBox::Retry
                                                                | QMessageBox::Cancel);
        if(ret == QMessageBox::Cancel)
            return;
    }

    serverStatusLabel->setText(tr("Listening"));
    clientStatusLabel->setText(tr("Connecting"));
    tcpClient.connectToHost(QHostAddress::LocalHost, tcpServer.serverPort());
}


void Dialog::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    if(!tcpServerConnection) {
        serverStatusLabel->setText(tr("Error: got invalid pending connection!"));
        return;
    }

    connect(tcpServerConnection, &QIODevice::readyRead,
            this, &Dialog::updateServerProgress);
    connect(tcpServerConnection,
            QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &Dialog::displayError);
    connect(tcpServerConnection, &QTcpSocket::disconnected,
            tcpServerConnection, &QTcpSocket::deleteLater);

    serverStatusLabel->setText(tr("Accepted connection"));
    tcpServer.close();
}


void Dialog::startTransfer()
{
    //当tcp客户端连接到环回服务器时调用
    bytesToWrite = TotalBytes - int(tcpClient.write(QByteArray(PayloadSize, '@')));
    clientStatusLabel->setText(tr("Connected"));
}


void Dialog::updateServerProgress()
{
    bytesReceived += int(tcpServerConnection->bytesAvailable());
    tcpServerConnection->readAll();

    serverProgressBar->setMaximum(TotalBytes);
    serverProgressBar->setValue(bytesReceived);
    serverStatusLabel->setText(tr("Received %1MB")
                               .arg(bytesReceived / (1024 * 1024)));

    if(bytesReceived == TotalBytes) {
        tcpServerConnection->close();
        startButton->setEnabled(true);

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
    }
}



void Dialog::updateClientProgress(qint64 numBytes)
{
    //当tcp客户端编写了一些字节时关闭
    bytesWritten += int(numBytes);

    //仅在未完成且qt写入缓冲区低于一定大小时才写入更多内容
    if(bytesToWrite > 0 && tcpClient.bytesToWrite() <= 4 * PayloadSize)
        bytesToWrite -= tcpClient.write(QByteArray(qMin(bytesToWrite, PayloadSize), '@'));

    clientProgressBar->setMaximum(TotalBytes);
    clientProgressBar->setValue(bytesWritten);
    clientStatusLabel->setText(tr("Sent %1MB").arg(bytesWritten / (1024 * 1024)));

}


void Dialog::displayError(QAbstractSocket::SocketError socketError)
{
    if(socketError == QTcpSocket::RemoteHostClosedError)
        return;

    QMessageBox::information(this, tr("Network error"),
                             tr("The following error occurred: %1.")
                             .arg(tcpClient.errorString()));

    tcpClient.close();
    tcpServer.close();
    clientProgressBar->reset();
    serverProgressBar->reset();
    clientStatusLabel->setText(tr("Client ready"));
    serverStatusLabel->setText(tr("Server ready"));
    startButton->setEnabled(true);
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif


}
