#ifndef BLOCKINGCLIENT_H
#define BLOCKINGCLIENT_H

#include <QWidget>

#include "fortunethread.h"

QT_BEGIN_NAMESPACE
class  QDialogButtonBox;
class  QLabel;
class  QLineEdit;
class  QPushButton;
class  QAction;
QT_END_NAMESPACE;



class BlockingClient : public QWidget
{
    Q_OBJECT

public:
    BlockingClient(QWidget *parent = 0);

private slots:
    void requestNewFortune();
    void showFortune(const QString &fortune);
    void displayError(int socketError, const QString &message);
    void enableGetFortuneButton();

private:
    QLabel *hostLabel;
    QLabel *portLabel;
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLabel *statusLabel;
    QPushButton *getFortuneButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    FortuneThread thread;
    QString currentFortune;
};

#endif // BLOCKINGCLIENT_H
