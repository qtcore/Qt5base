#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>

QT_BEGIN_NAMESPACE
class  QFile;
class  QLabel;
class  QLineEdit;
class  QPushButton;
class  QSslError;
class  QAuthenticator;
class  QNetworkReply;
class  QCheckBox;
QT_END_NAMESPACE

class  ProgressDialog : public QProgressDialog {
    Q_OBJECT

public:
    explicit ProgressDialog(const QUrl &url, QWidget *parent = nullptr);

public slots:
    void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);

};

class  HttpWindow : public QDialog
{
    Q_OBJECT

public:
    explicit HttpWindow(QWidget *parent = nullptr);
    void startRequest(const QUrl &requestedUrl);

private slots:
    void downloadFile();
    void cancelDownload();
    void httpFinished();
    void httpReadyRead();
    void enableDownloadButton();
    void slotAuthenticationRequired(QNetworkReply *, QAuthenticator *authenticator );
#ifndef QT_NO_SSL
    void sslErrors(QNetworkReply *, const QList<QSslError> &errors);
#endif


private:
    QFile *openFileForWrite(const QString &fileName);

    QLabel *statusLabel;
    QLineEdit *urlLineEdit;
    QPushButton *downloadButton;
    QCheckBox *launchCheckBox;
    QLineEdit *defaultFileLineEdit;
    QLineEdit *downlaodDirectoryLineEdit;

    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    bool httpRequestAborted;

};



#endif // MAINWINDOW_H
