#ifndef FORTUNETHREAD_H
#define FORTUNETHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QString>

class FortuneThread : public QThread
{
    Q_OBJECT

 public:
    FortuneThread(int socketDescriptor, const QString &fortune, QObject *parent);

    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor;
    QString text;

};

#endif // FORTUNETHREAD_H
