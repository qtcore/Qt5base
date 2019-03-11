#ifndef FORTUNESERVER_H
#define FORTUNESERVER_H

#include <QWidget>
#include <QStringList>
#include <QTcpServer>

class FortuneServer : public QTcpServer
{
    Q_OBJECT

public:
    FortuneServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QStringList fortunes;

};

#endif // FORTUNESERVER_H
