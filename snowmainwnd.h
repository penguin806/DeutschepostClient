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
    void onFetchingCaptchaImageFinished();
    void onSubmitLoginFormFinished();
    void onRequestPage_roduktauswahlJsp_Finished();

private:
    Ui::SnowMainWnd *ui;
    void startFetchingLoginPage();
    void startFetchingCaptchaImage();
    void startSubmitingLoginForm();
    void startRequestPage_roduktauswahlJsp();

    QNetworkAccessManager *manager;
    QNetworkReply *replyLoginPage;
    QNetworkReply *replyCaptchaImage;
    QString captchaText;
    QList<QNetworkCookie> cookies;
    QString _dynSessConf;
    QString _efiReqConf;

    QNetworkReply *replyLoginFormSubmitted;
    QNetworkReply *replyPage_roduktauswahlJsp;
};

#endif // SNOWMAINWND_H
