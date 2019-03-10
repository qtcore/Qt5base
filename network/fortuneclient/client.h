#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QDialog>
#include <QDataStream>
#include <QtNetwork/QTcpSocket>

QT_BEGIN_NAMESPACE
class  QComboBox;
class  QLabel;
class  QLineEdit;
class  QPushButton;
class  QTcpSocket;
class  QNetworkSession;
QT_END_NAMESPACE


class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void requestNewFortune();
    void readFortune();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetFortuneButton();
    void sessionOpened();

private:
    QComboBox *hostCombo = nullptr;
    QLineEdit *portLineEdit = nullptr;
    QLabel *statusLabel = nullptr;
    QPushButton *getFortuneButton = nullptr;

    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString currentFortune;

    QNetworkSession *networkSession = nullptr;

};

#endif // CLIENT_H
