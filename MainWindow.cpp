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
    setStatusFor(Reader);
    setCentralWidget(initWiget(rec));
    setStatusMsg(name + Widget::TU8("  登陆成功"));

    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(reLogin()));
    connect(ui->actionNewItem,SIGNAL(triggered(bool)),this,SLOT(createAction(bool)));
    connect(ui->actionChangeItem,SIGNAL(triggered(bool)),this,SLOT(alterAction(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStatusMsg(const QString& text, int timeout)
{
    qDebug()<<text;
    ui->statusBar->showMessage(text,timeout);
}

void MainWindow::setStatusMsg(const char* text, int timeout)
{
    qDebug()<<"setStatusMsg(const char* text, int timeout)";
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

void MainWindow::createAction(bool status)
{
    ui->actionFirstRow->setEnabled(!status);
    ui->actionLastRow->setEnabled(!status);
    ui->actionNextRow->setEnabled(!status);
    ui->actionPrevRow->setEnabled(!status);

    ui->actionDeleteItem->setEnabled(!status);
    ui->actionSubmit->setEnabled(!status);
    ui->actionChangePwd->setEnabled(!status);
    ui->actionExit->setEnabled(!status);
}
void MainWindow::alterAction(bool status)
{
    ui->actionNewItem->setEnabled(!status);
    ui->actionDeleteItem->setEnabled(!status);
    ui->actionSubmit->setEnabled(!status);
    ui->actionChangePwd->setEnabled(!status);
    ui->actionExit->setEnabled(!status);
}

Widget *MainWindow::initWiget(const QSqlRecord& rec)
{
    widget=nullptr;
    record = rec;
    int flag = rec.value(rec.indexOf("rdType")).toInt();
    if(flag == 1)//系统管理员
    {
        auto w = new ReaderWidget(this);
        connect(w,SIGNAL(statusMes(QString,int)),this,SLOT(setStatusMsg(QString,int)));
        w->setRecord(rec);
        setStatusFor(ReaderAdmin);
        widget = w;
        connectWidget();
    }
    else if(flag == 2)//借阅管理员
    {
        auto w = new BorrowWidget(this);
        w->setStatusFor(BorrowAdmin);
        setStatusFor(BorrowAdmin);
        w->setRecord(rec);
        connect(w,SIGNAL(statusMes(QString,int)),this,SLOT(setStatusMsg(QString,int)));
        widget = w;
        connectWidget();
    }
    else if(flag == 3)//图书管理员
    {
        auto w = new BookWidget(this);
        w->setStatusFor(BookAdmin);
        w->setRecord(rec);
        setStatusFor(BookAdmin);

        widget = w;
        connectWidget();

    }
    else//读者
    {
        auto w = new BorrowWidget(this);
        w->setStatusFor(Reader);
        setStatusFor(Reader);
        w->setRecord(rec);
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
    connect(widget,SIGNAL(statusMes(QString,int)),this,SLOT(setStatusMsg(QString,int)));
}

//设置工具栏状态
void MainWindow::setStatusFor(WidgetStatus status)
{
    if(status == ReaderAdmin)
    {
        setActionVisable(true);
        ui->actionNewItem->setText(Widget::TU8("办理借书证"));
        ui->actionChangeItem->setText(Widget::TU8("修改信息"));
        return;
    }
    else if(status == BookAdmin)
    {
        setActionVisable(true);
        ui->actionNewItem->setText(Widget::TU8("新书入库"));
        ui->actionChangeItem->setText(Widget::TU8("信息维护"));
        return;
    }
    setActionVisable(false);
}

void MainWindow::setActionVisable(bool status)
{
    ui->actionNewItem->setVisible(status);
    ui->actionChangeItem->setVisible(status);
    ui->actionSubmit->setVisible(status);
    ui->actionDeleteItem->setVisible(status);
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
