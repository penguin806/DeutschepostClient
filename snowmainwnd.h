#ifndef SNOWMAINWND_H
#define SNOWMAINWND_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class SnowMainWnd;
}

class SnowMainWnd : public QDialog
{
    Q_OBJECT

public:
    explicit SnowMainWnd(QWidget *parent = nullptr);
    ~SnowMainWnd();

private slots:
    void on_pushButton_clicked();
    void onRequestLoginPageFinished();
    void onSubmitLoginFormFinished();

private:
    Ui::SnowMainWnd *ui;
    void startLogin();
    void startSubmitingLoginForm();

    QNetworkAccessManager *manager;
    QNetworkReply *replyLoginPage;
    QList<QNetworkCookie> cookies;
    QString _dynSessConf;
    QString _efiReqConf;

    QNetworkReply *replyLoginFormSubmitted;
};

#endif // SNOWMAINWND_H
