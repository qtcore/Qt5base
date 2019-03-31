#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QObject>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void replyFinished(QNetworkReply*);

    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
