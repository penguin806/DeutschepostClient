#ifndef CAPTCHAIMAGEDIALOG_H
#define CAPTCHAIMAGEDIALOG_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class CaptchaImageDialog;
}

class CaptchaImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CaptchaImageDialog(QString &_captchaText, QWidget *parent = nullptr);
    ~CaptchaImageDialog();
    void setPixmap(QPixmap pixmap);

private slots:
    void on_pushButton_Ok_clicked();

private:
    Ui::CaptchaImageDialog *ui;
//    QPixmap pixmap;
    QString &captchaText;
};

#endif // CAPTCHAIMAGEDIALOG_H
