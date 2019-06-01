#include "snowmainwnd.h"
#include "ui_snowmainwnd.h"
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QRegularExpression>
#include <QVariant>
#include <QUrlQuery>
#include <QDebug>

SnowMainWnd::SnowMainWnd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SnowMainWnd)
{
    ui->setupUi(this);
    this->manager = new QNetworkAccessManager(this);
//    QObject::connect(this->manager,SIGNAL(),
//                     this,SLOT(onRequestFinished(QNetworkReply*)));
#ifdef QT_DEBUG
    qDebug() << "SSL Version:" << QSslSocket::sslLibraryBuildVersionString();
#endif
    this->ui->lineEdit_Username->setText("snowww");
    this->ui->lineEdit_Password->setText("*****");
}

SnowMainWnd::~SnowMainWnd()
{
    delete ui;
}

void SnowMainWnd::on_pushButton_clicked()
{
    this->ui->pushButton->setEnabled(false);
    this->startFetchingLoginPage();
}

void SnowMainWnd::onRequestLoginPageFinished()
{
    qDebug() << "onRequestLoginPageFinished(): " << this->replyLoginPage->bytesAvailable();
    this->ui->textEdit_Info->append("Response: " +
                                    QString::number(this->replyLoginPage->bytesAvailable()) + "Bytes");

    QNetworkCookieJar *cookieJar = this->manager->cookieJar();
    this->cookies = cookieJar->cookiesForUrl(QUrl(
                "shop.deutschepost.de"));
    for(int i=0; i< this->cookies.length(); i++)
    {
       qDebug() << this->cookies.at(i).name() << ": "
                << this->cookies.at(i).value();
    }

    QString responseData = QString::fromUtf8(this->replyLoginPage->readAll());
    qDebug() << responseData;

    QRegularExpression expInput1("<input name=\"_dynSessConf\" type=\"hidden\" value=\"(.*?)\">.*?<input type=\"hidden\" name=\"_efiReqConf\" value=\"(.*)\">");
    QRegularExpressionMatch match = expInput1.match(responseData);
    this->_dynSessConf = match.captured(1);
    this->_efiReqConf = match.captured(2);
    qDebug () << "_dynSessConf: " << this->_dynSessConf;
    this->ui->textEdit_Info->append("_dynSessConf: " + this->_dynSessConf);
    qDebug () << "_efiReqConf: " << this->_efiReqConf;
    this->ui->textEdit_Info->append("_efiReqConf: " + this->_efiReqConf);

    this->replyLoginPage->deleteLater();
    this->replyLoginPage = nullptr;

    // Stage 2: POST login form
    //return;
    this->startSubmitingLoginForm();
}

void SnowMainWnd::onSubmitLoginFormFinished()
{
    qDebug() << "onSubmitLoginFormFinished(): " << this->replyLoginFormSubmitted->bytesAvailable();
    this->ui->textEdit_Info->append("Response: " +
                                    QString::number(this->replyLoginFormSubmitted->bytesAvailable()) + "Bytes");
    qDebug() << "Status: " +
                this->replyLoginFormSubmitted->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    this->ui->textEdit_Info->append("HttpReasonPhraseAttribute: " + this->replyLoginFormSubmitted->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString());


    QNetworkCookieJar *cookieJar = this->manager->cookieJar();
    this->cookies = cookieJar->cookiesForUrl(QUrl(
                "shop.deutschepost.de"));
    for(int i=0; i< this->cookies.length(); i++)
    {
       qDebug() << this->cookies.at(i).name() << ": "
                << this->cookies.at(i).value();
    }

    QString responseData =
            QString::fromUtf8(this->replyLoginFormSubmitted->readAll());
    qDebug() << "responseData: " + responseData;

    // Login Finished
    QString redirectionTarget =
            this->replyLoginFormSubmitted->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
    qDebug() << "RedirectionTarget: " + redirectionTarget;


    if(this->replyLoginFormSubmitted->attribute(
                QNetworkRequest::HttpStatusCodeAttribute).toInt() == 302)
    {
        this->replyLoginFormSubmitted->deleteLater();
        this->replyLoginFormSubmitted = nullptr;

        // Login Success
        this->ui->textEdit_Info->append("Login Success!");

        // Stage 3: Request other pages
        this->startRequestPage_roduktauswahlJsp();
    }
    else
    {
        this->replyLoginFormSubmitted->deleteLater();
        this->replyLoginFormSubmitted = nullptr;

        // Login Failed
        this->ui->textEdit_Info->append("Login Failed!");
    }
}

void SnowMainWnd::onRequestPage_roduktauswahlJsp_Finished()
{
    qDebug() << "onRequestPage_roduktauswahlJsp_Finished(): " << this->replyPage_roduktauswahlJsp->bytesAvailable();
    this->ui->textEdit_Info->append("Response: " +
                                    QString::number(this->replyPage_roduktauswahlJsp->bytesAvailable()) + "Bytes");
    qDebug() << "Status: " +
                this->replyPage_roduktauswahlJsp->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    this->ui->textEdit_Info->append("HttpReasonPhraseAttribute: " + this->replyPage_roduktauswahlJsp->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString());

    QString responseData =
            QString::fromUtf8(this->replyPage_roduktauswahlJsp->readAll());
    qDebug() << "responseData: " + responseData;
}

void SnowMainWnd::startFetchingLoginPage()
{
    // Stage 1: GET and parse the login html
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::UserAgentHeader,"Snow-Client admin@xuefeng.space");
    request.setUrl(QUrl("https://shop.deutschepost.de/shop/login_page.jsp"));

    this->ui->textEdit_Info->append("GET: https://shop.deutschepost.de/shop/login_page.jsp");
    this->replyLoginPage = this->manager->get(request);
    QObject::connect(this->replyLoginPage, SIGNAL(finished()),
                     this,SLOT(onRequestLoginPageFinished()));

}

void SnowMainWnd::startSubmitingLoginForm()
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://shop.deutschepost.de/shop/login_page.jsp"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader,"Snow-Client admin@xuefeng.space");
    QVariant var;
    var.setValue(this->cookies);
    request.setHeader(QNetworkRequest::CookieHeader,var);


    QUrlQuery queryParams;
    queryParams.addQueryItem("_dynSessConf",this->_dynSessConf);
    queryParams.addQueryItem("_efiReqConf",this->_efiReqConf);
    queryParams.addQueryItem("username",this->ui->lineEdit_Username->text());
    queryParams.addQueryItem("_D:username","+");
    queryParams.addQueryItem("password",this->ui->lineEdit_Password->text());
    queryParams.addQueryItem("_D:password","+");
    queryParams.addQueryItem("/de/dpag/efiliale/presentation/login/LoginFormHandler.submitme","true");
    queryParams.addQueryItem("_D:/de/dpag/efiliale/presentation/login/LoginFormHandler.submitme","+");
    queryParams.addQueryItem("/de/dpag/efiliale/presentation/login/LoginFormHandler.targetLocation","");
    queryParams.addQueryItem("_D:/de/dpag/efiliale/presentation/login/LoginFormHandler.targetLocation","+");
    queryParams.addQueryItem("_DARGS","/shop/includes/loginform/loginform.jsp");

    this->ui->textEdit_Info->append("POST: https://shop.deutschepost.de/shop/login_page.jsp");
    this->replyLoginFormSubmitted =
            this->manager->post(request,queryParams.query().toUtf8());
    QObject::connect(this->replyLoginFormSubmitted,SIGNAL(finished()),
                     this,SLOT(onSubmitLoginFormFinished()));
}

void SnowMainWnd::startRequestPage_roduktauswahlJsp()
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://shop.deutschepost.de/shop/warenpost/produktauswahl.jsp"));
    request.setHeader(QNetworkRequest::UserAgentHeader,"Snow-Client admin@xuefeng.space");
    QVariant var;
    var.setValue(this->cookies);
    request.setHeader(QNetworkRequest::CookieHeader,var);

    this->ui->textEdit_Info->append("GET: https://shop.deutschepost.de/shop/warenpost/produktauswahl.jsp");
    this->replyPage_roduktauswahlJsp = this->manager->get(request);
    QObject::connect(this->replyPage_roduktauswahlJsp,SIGNAL(finished()),
                     this,SLOT(onRequestPage_roduktauswahlJsp_Finished()));
}
