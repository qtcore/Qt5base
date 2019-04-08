#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QFile>
#include <QProgressBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void upload();    //上传
   void download();  //下载
   void readContent();  //下载内容写入文件
    void replyfinshed(QNetworkReply*);  //下载完成
    void loadError(QNetworkReply::NetworkError);  //下载时错误提示
    void loadProgress(qint64 bytesSent, qint64 bytesTotal);

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QNetworkRequest *quest;
    QFile *file;
    QProgressBar *progressBar;


};

#endif // MAINWINDOW_H
