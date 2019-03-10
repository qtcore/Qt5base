#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QString>
#include <QVector>

QT_BEGIN_NAMESPACE
class  QLabel;
class  QTcpServer;
class  QNetworkSession;
QT_END_NAMESPACE



class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);


private slots:
    void sessionOpened();
    void sendFortune();

private:
    QLabel *statusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    QVector<QString> fortunes;
    QNetworkSession *networkSession = nullptr;

};

#endif // SERVER_H
