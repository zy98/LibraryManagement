#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSqlRecord>
#include "Database.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    //用于登陆成功后获取登陆者信息
    QSqlRecord get_info();

private slots:
    //登陆按钮，在登陆按钮中首次打开数据库，后续使用的都是此连接的副本
    void on_btn_login_clicked();

private:
    //设置错误信息，并转换为UTF8
    void setMsgText(const char* s = "");

    //设置定时消息，调用setMsgText设置错误信息
    void setTimingText(const char* s = "");

private:
    Ui::LoginDialog *ui;
    QTimer* timer;
    QSqlRecord record;
};

#endif // LOGINDIALOG_H
