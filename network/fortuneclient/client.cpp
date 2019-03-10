#include "client.h"
#include <QtWidgets>
#include <QtNetwork/QtNetwork>



Client::Client(QWidget *parent)
    : QWidget(parent)
    , hostCombo(new QComboBox)
    , portLineEdit(new QLineEdit)
    , getFortuneButton(new QPushButton(tr("Get Fortune")))
    , tcpSocket(new QTcpSocket(this))

{
     setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);

     hostCombo->setEditable(true);

     //找出本地主机名称
     QString name = QHostInfo::localHostName();
     if(!name.isEmpty()){
         hostCombo->addItem(name);
         QString domain = QHostInfo::localDomainName();
         if(!domain.isEmpty())
             hostCombo->addItem(name + QChar('.') + domain);
     }
     if(name != QLatin1String("localhost"))
         hostCombo->addItem(QString("localhost"));

     //找出本地主机的IP地址
     QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

     //添加非本地主机地址
     for(int i=0; i<ipAddressesList.size(); ++i) {
         if(!ipAddressesList.at(i).isLoopback())
             hostCombo->addItem(ipAddressesList.at(i).toString());

     }

     //添加本地主机地址
     for(int i=0; i<ipAddressesList.size(); ++i) {
         if(ipAddressesList.at(i).isLoopback())
                 hostCombo->addItem(ipAddressesList.at(i).toString());
     }

     portLineEdit->setValidator(new QIntValidator(1, 65535, this));

     auto hostLabel = new QLabel(tr("&Server name:"));
     hostLabel->setBuddy(hostCombo);
     auto portLabel = new QLabel(tr("S&erver port:"));
     portLabel->setBuddy(portLabel);

     statusLabel = new QLabel(tr("This example requires that you run the "
                                 "Fortune Server example as well."));

     getFortuneButton->setDefault(true);
     getFortuneButton->setEnabled(false);

     auto quitButton = new QPushButton(tr("Quit"));

     auto buttonBox = new QDialogButtonBox;
     buttonBox->addButton(getFortuneButton, QDialogButtonBox::ActionRole);
     buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

     in.setDevice(tcpSocket);
     in.setVersion(QDataStream::Qt_5_9);


     connect(hostCombo, &QComboBox::editTextChanged,
             this, &Client::enableGetFortuneButton);

     connect(portLineEdit, &QLineEdit::textChanged,
             this, &Client::enableGetFortuneButton);

     connect(getFortuneButton, &QAbstractButton::clicked,
             this, &Client::requestNewFortune);

     connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);

     connect(tcpSocket, &QIODevice::readyRead, this, &Client::readFortune);

     connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
             this, &Client::displayError);


     QGridLayout *mainLayout = nullptr;
        if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
            auto outerVerticalLayout = new QVBoxLayout(this);
            outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
            auto outerHorizontalLayout = new QHBoxLayout;
            outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
            auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
            mainLayout = new QGridLayout(groupBox);
            outerHorizontalLayout->addWidget(groupBox);
            outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
            outerVerticalLayout->addLayout(outerHorizontalLayout);
            outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        } else {
            mainLayout = new QGridLayout(this);
        }
        mainLayout->addWidget(hostLabel, 0, 0);
        mainLayout->addWidget(hostCombo, 0, 1);
        mainLayout->addWidget(portLabel, 1, 0);
        mainLayout->addWidget(portLineEdit, 1, 1);
        mainLayout->addWidget(statusLabel, 2, 0, 1, 2);
        mainLayout->addWidget(buttonBox, 3, 0, 1, 2);

        setWindowTitle(QGuiApplication::applicationDisplayName());
        portLineEdit->setFocus();


        QNetworkConfigurationManager manager;
        if(manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
            //获取已保存的网络配置
            QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
            settings.beginGroup(QLatin1String("QtNetwork"));
            const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
            settings.endGroup();

            //如果没有发现当前保存的网络配置，则使用系统默认
            QNetworkConfiguration config = manager.configurationFromIdentifier(id);
            if((config.state() & QNetworkConfiguration::Discovered) !=
                    QNetworkConfiguration::Discovered) {
                config = manager.defaultConfiguration();
            }


            networkSession = new QNetworkSession(config, this);
            connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);

            getFortuneButton->setEnabled(false);
            statusLabel->setText(tr("Opending network session"));
            networkSession->open();

        }


}


void Client::requestNewFortune()
{
    getFortuneButton->setEnabled(false);
    tcpSocket->abort();

    tcpSocket->connectToHost(hostCombo->currentText(),
                             portLineEdit->text().toUShort());

}


void Client::readFortune()
{
    in.startTransaction();

    QString nextFortune;
    in >> nextFortune;

    if(!in.commitTransaction())
        return;

    if(nextFortune == currentFortune) {
        QTimer::singleShot(0, this, &Client::requestNewFortune);
        return;
    }


    currentFortune = nextFortune;
    statusLabel->setText(currentFortune);
    getFortuneButton->setEnabled(true);

}


void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                           "host name and port settings."));
               break;
           case QAbstractSocket::ConnectionRefusedError:
               QMessageBox::information(this, tr("Fortune Client"),
                                        tr("The connection was refused by the peer. "
                                           "Make sure the fortune server is running, "
                                           "and check that the host name and port "
                                           "settings are correct."));
               break;
           default:
               QMessageBox::information(this, tr("Fortune Client"),
                                        tr("The following error occurred: %1.")
                                        .arg(tcpSocket->errorString()));

    }

    getFortuneButton->setEnabled(true);
}



void Client::enableGetFortuneButton()
{
    getFortuneButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                 !hostCombo->currentText().isEmpty() &&
                                 !portLineEdit->text().isEmpty());
}



void Client::sessionOpened()
{
    //使用保存的配置
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if(config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    statusLabel->setText(tr("This example requires that you run the "
                            "Fortune Server example as well."));

    enableGetFortuneButton();
}
