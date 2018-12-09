#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "Widget.h"
#include "ReaderWidget.h"
#include "LoginDialog.h"
#include "BookWidget.h"
#include "BorrowWidget.h"

MainWindow::MainWindow(QWidget *parent, const QSqlRecord &rec) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1550,780);
    QString name = rec.value(rec.indexOf("rdName")).toString();
    setCentralWidget(initWiget(rec));
    setStatusMsg(name + Widget::TU8("  登陆成功"));

    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(reLogin()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStatusMsg(const QString& text, int timeout)
{
    ui->statusBar->showMessage(text,timeout);
}

void MainWindow::setStatusMsg(const char* text, int timeout)
{
    setStatusMsg(QString::fromLocal8Bit(text),timeout);
}

void MainWindow::reLogin()
{
    widget->close();
    this->hide();

    LoginDialog dialog;
    if(dialog.exec() != QDialog::Accepted)
    {
        //qApp->quit();
        this->close();
        return;
    }

    auto rec = dialog.get_info();
    this->setCentralWidget(initWiget(rec));
    this->show();
}

Widget *MainWindow::initWiget(const QSqlRecord& rec)
{
    widget=nullptr;
    record = rec;
    int flag = rec.value(rec.indexOf("rdType")).toInt();
    if(flag == 1)
    {
        auto w = new ReaderWidget(this);
        w->setRecord(rec);
        setStatusFor(ReaderAdmin);
        widget = w;
        connectWidget();
    }
    else if(flag == 2)
    {
        auto w = new BorrowWidget(this);
        w->setStatusFor(BorrowAdmin);
        w->setRecord(rec);
        setStatusFor(BorrowAdmin);
        widget = w;
        connectWidget();
    }
    else if(flag == 3)
    {
        auto w = new BookWidget(this);
        w->setStatusFor(BookAdmin);
        w->setRecord(rec);
        setStatusFor(BookAdmin);
        widget = w;
        connectWidget();
    }
    else
    {
        auto w = new BorrowWidget(this);
        w->setStatusFor(Reader);
        w->setRecord(rec);
        setStatusFor(Reader);
        widget = w;
        connectWidget();
    }

    if(!widget) widget = new Widget(this);
    return widget;
}

//连接工具栏信号与自定义信号
void MainWindow::connectWidget()
{
    connect(ui->actionFirstRow,SIGNAL(triggered()),widget,SLOT(first()));
    connect(ui->actionLastRow,SIGNAL(triggered()),widget,SLOT(last()));
    connect(ui->actionPrevRow,SIGNAL(triggered()),widget,SLOT(prev()));
    connect(ui->actionNextRow,SIGNAL(triggered()),widget,SLOT(next()));
    connect(ui->actionNewItem,SIGNAL(triggered(bool)),widget,SLOT(newItem(bool)));
    connect(ui->actionChangeItem,SIGNAL(triggered(bool)),widget,SLOT(changeItem(bool)));
    connect(ui->actionSubmit,SIGNAL(triggered()),widget,SLOT(submitItem()));
    connect(ui->actionDeleteItem,SIGNAL(triggered()),widget,SLOT(deleteItem()));
    connect(ui->actionChangePwd,SIGNAL(triggered()),widget,SLOT(changePwd()));
}

//设置工具栏状态
void MainWindow::setStatusFor(WidgetStatus status)
{
    if(status == BookAdmin || status == ReaderAdmin)
    {
        ui->actionNewItem->setVisible(true);
        ui->actionChangeItem->setVisible(true);
        ui->actionSubmit->setVisible(true);
        ui->actionDeleteItem->setVisible(true);
        return;
    }
    ui->actionNewItem->setVisible(false);
    ui->actionChangeItem->setVisible(false);
    ui->actionSubmit->setVisible(false);
    ui->actionDeleteItem->setVisible(false);
}

void MainWindow::on_actionChangePwd_triggered()
{
    QString srcPwd = QInputDialog::getText(this,Widget::TU8("修改密码"),Widget::TU8("原密码："),
                                           QLineEdit::Password);
    if(srcPwd == "") return;

    QString newPwd = QInputDialog::getText(this,Widget::TU8("修改密码"),Widget::TU8("新密码："),
                                           QLineEdit::PasswordEchoOnEdit);
    if(newPwd == "") return;

    if(srcPwd != newPwd)
    {
        QSqlDatabase db = QSqlDatabase::database("Library");

        QSqlQuery query(db);

        query.prepare("exec proc_update_pwd ?, ?, ?");
        query.addBindValue(record.value(0));
        query.addBindValue(srcPwd);
        query.addBindValue(newPwd);

        if(!query.exec())
            setStatusMsg(query.lastError().databaseText(),3000);

        else
            setStatusMsg("修改密码成功！");
        return;
    }

    setStatusMsg("新密码与原密码相同，已取消修改！",4000);
}
