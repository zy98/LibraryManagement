#include "LoginDialog.h"
#include "ui_LoginDialog.h"

#include "Database.h"
#include <QDebug>
#include <QSqlQuery>
#include <QTimer>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::LoginDialog), timer(new QTimer(this))
{
    ui->setupUi(this);
    connect(ui->btn_cancel,SIGNAL(clicked()),this,SLOT(close()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

inline
void LoginDialog::setMsgText(const char* s)
{
    ui->lb_msg->setText(QString::fromLocal8Bit(s));
}

inline
void LoginDialog::setTimingText(const char* s)
{
    setMsgText(s);
    timer->start(5000);
}

QSqlRecord LoginDialog::get_info()
{
    return record;
}

void LoginDialog::on_btn_login_clicked()
{
    auto user = ui->edit_user->text();
    auto pwd = ui->edit_pwd->text();

    if(user == "" || pwd ==  "")
    {
        setTimingText("用户编号不能为空！");
        return ;
    }

    auto db = QSqlDatabase::database("Library");
    if(!db.isOpen())
    {
        setTimingText("连接数据库失败！");
        return;
    }

    QSqlQuery query(db);
    query.prepare("select * from Reader where rdID = :user");
    query.addBindValue(user);

    if(query.exec() && query.first())
    {
        record = query.record();
        if(record.value(1).toString() == pwd)
            accept();
        else
            setTimingText("用户名或密码错误！");
    }
    else
    {
        setTimingText("获取数据失败！");
    }
}
