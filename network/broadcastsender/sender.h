#ifndef SENDER_H
#define SENDER_H

#include <QWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE
class  QLabel;
class  QPushButton;
class  QUdpSocket;
QT_END_NAMESPACE


class Sender : public QWidget
{
    Q_OBJECT

public:
    explicit Sender(QWidget *parent = nullptr);

private slots:
    void startBroadcasting(); //开始广播
    void broadcastDatagram(); //广播数据报
    void stopBroadcasting();  //停止广播

private:
    QLabel *statusLabel = nullptr;
    QPushButton *startButton = nullptr;
    QUdpSocket *udpSocket = nullptr;
    QTimer timer;
    int messageNo = 1;
    QPushButton *stopButton = nullptr;
};

#endif // SENDER_H
