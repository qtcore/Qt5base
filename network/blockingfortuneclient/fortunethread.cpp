#include <QtNetwork>
#include "fortunethread.h"

FortuneThread::FortuneThread(QObject *parent)
     : QThread(parent), quit(false)
{

}



FortuneThread::~FortuneThread()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}


void FortuneThread::requestNewFortune(const QString &hostName, quint16 port)
{
    QMutexLocker locker(&mutex);
    this->hostName = hostName;
    this->port = port;

    if(!isRunning())
        start();
    else {
        cond.wakeOne();  //等待唤醒线程
    }
}


void FortuneThread::run()
{
    mutex.lock();

    QString serverName = hostName;
    quint16 serverPort = port;
    mutex.unlock();

    while(!quit) {
        const int Timeout = 5*1000;

        QTcpSocket socket;
        socket.connectToHost(serverName,serverPort);

        if(!socket.waitForConnected(Timeout)) {
            emit error(socket.error(), socket.errorString());
            return;
        }

        QDataStream in(&socket);
        in.setVersion(QDataStream::Qt_4_0);
        QString fortune;

        do{
            if(!socket.waitForReadyRead(Timeout)) {
                emit error(socket.error(), socket.errorString());
                return;
            }

            in.startTransaction();
            in >> fortune;
        } while(!in.commitTransaction());

        mutex.lock();
        emit newFortune(fortune);

        cond.wait(&mutex);
        serverName = hostName;
        serverPort = port;
        mutex.unlock();

    }
}
