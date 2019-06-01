#include "captchaimagedialog.h"
#include "ui_captchaimagedialog.h"

CaptchaImageDialog::CaptchaImageDialog(QString &_captchaText, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaptchaImageDialog),
    captchaText(_captchaText)
{
    ui->setupUi(this);
}

CaptchaImageDialog::~CaptchaImageDialog()
{
    delete ui;
}

void CaptchaImageDialog::setPixmap(QPixmap pixmap)
{
    this->ui->label_CaptchaImage->setPixmap(pixmap);
}

void CaptchaImageDialog::on_pushButton_Ok_clicked()
{
    this->captchaText = this->ui->lineEdit_CaptchaText->text();
    this->close();
}
