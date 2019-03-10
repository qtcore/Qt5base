#include "blockingclient.h"
#include <QtWidgets>
#include <QtNetwork>


BlockingClient::BlockingClient(QWidget *parent)
    : QWidget(parent)
{

    hostLabel = new QLabel(tr("&Server name:"));
    portLabel = new QLabel(tr("S&erver port:"));

    //找出连接到哪个IP
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    //使用第一个非本地主机ipv4地址
    for(int i=0; i<ipAddressesList.size(); ++i) {
        if(ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {

            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    //如果我们没有找到，使用ipv4本地主机
    if(!ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    hostLineEdit = new QLineEdit(ipAddress);
    portLineEdit = new QLineEdit;
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    hostLabel->setBuddy(hostLineEdit);
    portLabel->setBuddy(portLineEdit);

    statusLabel = new QLabel(tr("This example requires that you run the "
                                "Fortune Server example as well."));
    statusLabel->setWordWrap(true);

    getFortuneButton = new QPushButton(tr("Get Fortune"));
    getFortuneButton->setDefault(true);
    getFortuneButton->setEnabled(false);

    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(getFortuneButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(getFortuneButton, SIGNAL(clicked()), this, SLOT(requestNewFortune()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    connect(hostLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(enableGetFortuneButton()));
    connect(portLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(showFortune(QString)));

    connect(&thread, SIGNAL(newFortune(QString)),
            this, SLOT(showFortune(QString)));

    connect(&thread, SIGNAL(error(int, QString)),
            this, SLOT(displayError(int, QString)));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostLineEdit, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(statusLabel, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("Blocking Fortune Client"));
    portLineEdit->setFocus();

}


void BlockingClient::requestNewFortune()
{
    getFortuneButton->setEnabled(false);
    thread.requestNewFortune(hostLineEdit->text(),
                             portLineEdit->text().toUShort());
}


void BlockingClient::showFortune(const QString &nextFortune)
{
    if(nextFortune == currentFortune) {
        requestNewFortune();
        return;
    }

    currentFortune = nextFortune;
    statusLabel->setText(currentFortune);
    getFortuneButton->setEnabled(true);
}


void BlockingClient::displayError(int socketError, const QString &message)
{
    switch (socketError) {
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Blocking Fortune CLient"),
                                 tr("The host was not found. Please check the "
                                    "host and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Blocking Fortuen Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Blocking Fortune Client"),
                                 tr("The follwing error occurred: %1.")
                                 .arg(message));

    }

    getFortuneButton->setEnabled(true);
}


void BlockingClient::enableGetFortuneButton()
{
    bool enable(!hostLineEdit->text().isEmpty() && !portLineEdit->text().isEmpty());
    getFortuneButton->setEnabled(enable);
}
