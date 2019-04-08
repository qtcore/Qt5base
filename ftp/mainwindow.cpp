#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QFile>
#include <QProgressBar>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //上传
    connect(ui->upload, SIGNAL(clicked()), this, SLOT(upload()));

    //下载
    connect(ui->download,SIGNAL(clicked()), this, SLOT(download()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::upload()
{
    //创建并打开文件
    file = new QFile("d:/qt5/ftp2/qt.jpg");
    file->open(QIODevice::ReadOnly);
    //读取文件数据
    QByteArray byte_file = file->readAll();

    //创建网络访问管理器对象
    manager = new QNetworkAccessManager(this);

    //设置网络访问属性-- 允许访问
    manager->setNetworkAccessible(QNetworkAccessManager::Accessible);

    //创建URL-指定FTP服务器地址
    QUrl url("ftp://your_server" + ui->lineEdit->text() + ".jpg");

    //设置FTP端口，用户名，密码
    url.setPort(21);
    url.setUserName("your_name");
    url.setPassword("your_password");

    //创建网络请求对象
    QNetworkRequest request(url);

    //发送网络请求，并接收回复
    reply = manager->put(request,byte_file);
    //创建进度条对象
  progressBar = new QProgressBar();
   progressBar->setValue(0);
   progressBar->show();

   connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyfinshed(QNetworkReply*)));

   connect(reply,SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(loadError(QNetworkReply::NetworkError)));

   connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(loadProgress(qint64, qint64)));


}

void MainWindow::download()
{
    //保存下载的文件路径
    file = new QFile(QApplication::applicationDirPath() + "/" + ui->lineEdit->text() + ".jpg");
    file->open(QIODevice::WriteOnly); //以写入方式打开文件

    //创建网络访问管理器对象
    manager = new QNetworkAccessManager(this);
    manager->setNetworkAccessible(QNetworkAccessManager::Accessible);

    //创建URL-指定FTP服务器地址
    QUrl url("ftp://your_server/123.jpg");

    //设置FTP端口，用户名，密码
    url.setPort(21);
    url.setUserName("your_name");
    url.setPassword("your_password");

    //网络请求-
     QNetworkRequest request(url);
     reply = manager->get(request);

      progressBar = new QProgressBar();
         progressBar->setValue(0);
         progressBar->show();

         //写入文件数据
         connect(reply, SIGNAL(readyRead()), this, SLOT(readContent()));
         //网络上传下载完成信号
         connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyfinshed(QNetworkReply*)));
         //网络传输过程中错误提示信号
         connect(reply,SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(loadError(QNetworkReply::NetworkError)));
         //下载更新进度条
         connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(loadProgress(qint64, qint64)));

}

void MainWindow::readContent()  //下载时向本地文件写入数据
{
    file->write(reply->readAll());


}

void MainWindow::replyfinshed(QNetworkReply*)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        reply->deleteLater(); //删除对象
        file->flush();
        file->close();
    }
    else {
        QMessageBox::critical(nullptr, tr("Error"), "Failed!!!");
    }

}


void MainWindow::loadError(QNetworkReply::NetworkError)
{
    qDebug() <<"Error:" << reply->errorString();
}


void MainWindow::loadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug() << "loaded " << bytesSent << " of " << bytesTotal;


    progressBar->setMaximum(bytesTotal); //最大值
    progressBar->setValue(bytesSent); //当前值
}
