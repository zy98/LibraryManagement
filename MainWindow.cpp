#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

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
    this->hide();
    widget->close();

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
    int flag = 0;
    flag = rec.value(rec.indexOf("rdType")).toInt();

    if(flag == 1)
    {
        auto w = new ReaderWidget(this);
        w->setRecord(rec);
        widget = w;
        connectWidget();
    }
    else if(flag == 2)
    {
        auto w = new BorrowWidget(this);
        w->setRecord(rec);
        widget = w;
        connectWidget();
    }
    else if(flag == 3)
    {
        auto w = new BookWidget(this);
        w->setRecord(rec);
        widget = w;
        connectWidget();
    }
    else
    {
        widget = new Widget(this);
        widget->setRecord(rec);
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

    //connect(ui->actionExit,SIGNAL(triggered()),widget,SLOT(close()));
}
