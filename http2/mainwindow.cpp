#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //简单的HTTP下载
    //创建网络访问管理器对象
    manager = new QNetworkAccessManager(this);




    reply = manager->get(QNetworkRequest(QUrl("http://www.wowanc.com")));;


    connect(manager,&QNetworkAccessManager::finished,this, &MainWindow::replyFinished);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::replyFinished(QNetworkReply*)

{
    //reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
   // reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

  QByteArray bytes = reply->readAll();
  QString result= QString(bytes);
  qDebug() << result;

    ui->textEdit->setText(result);

   reply->deleteLater();


}
