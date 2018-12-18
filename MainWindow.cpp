#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "UI/Widget.h"
#include "UI/ReaderWidget.h"
#include "LoginDialog.h"
#include "UI/BookWidget.h"
#include "UI/BorrowWidget.h"

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

    ui->actionChangeItem->setEnabled(!status);
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
        widget = new ReaderWidget(this);
        setStatusFor(ReaderAdmin);
        connect(widget,SIGNAL(statusMes(QString,int)),this,SLOT(setStatusMsg(QString,int)));
    }
    else if(flag == 2)//借阅管理员
    {
        widget = new BorrowWidget(this);
        setStatusFor(BorrowAdmin);
        connect(widget,SIGNAL(statusMes(QString,int)),this,SLOT(setStatusMsg(QString,int)));
    }
    else if(flag == 3)//图书管理员
    {
        widget = new BookWidget(this);
        setStatusFor(BookAdmin);
    }
    else//读者
    {
        widget = new BorrowWidget(this);
        setStatusFor(Reader);
    }

    widget->setRecord(rec);
    connectWidget();

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
        widget->setStatusFor(ReaderAdmin);
        setActionVisable(true);
        ui->actionNewItem->setText(Widget::TU8("办理借书证"));
        ui->actionChangeItem->setText(Widget::TU8("修改信息"));
        return;
    }
    else if(status == BookAdmin)
    {
        widget->setStatusFor(BookAdmin);
        setActionVisable(true);
        ui->actionNewItem->setText(Widget::TU8("新书入库"));
        ui->actionChangeItem->setText(Widget::TU8("信息维护"));
        return;
    }
    else if (status == BorrowAdmin)
    {
        widget->setStatusFor(BorrowAdmin);
        setActionVisable(false);
    }
    else
    {
        widget->setStatusFor(Reader);
        setActionVisable(false);
    }
}

void MainWindow::setActionVisable(bool status)
{
    ui->actionNewItem->setVisible(status);
    ui->actionChangeItem->setVisible(status);
    ui->actionSubmit->setVisible(status);
    ui->actionDeleteItem->setVisible(status);
}


