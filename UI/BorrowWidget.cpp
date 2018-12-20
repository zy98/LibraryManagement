#include "BorrowWidget.h"
#include "ui_BorrowWidget.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

BorrowWidget::BorrowWidget(QWidget *parent) :
    Widget(parent),
    ui(new Ui::BorrowWidget)
{
    ui->setupUi(this);
    ui->borrow->setStatusFor(BorrowAdmin);
    ui->reader->setStatusFor(BorrowAdmin);
    ui->book->setStatusFor(BorrowAdmin);

    connect(ui->book,SIGNAL(statusMes(const QString&,int)),this,SIGNAL(statusMes(const QString&,int)));
    connect(ui->book,SIGNAL(borrowBook(long long)),this,SLOT(BorrowBook(long long)));
    connect(ui->borrow,SIGNAL(returnBook(QItemSelectionModel*)),this,SLOT(ReturnBook(QItemSelectionModel*)));
}

BorrowWidget::~BorrowWidget()
{
    delete ui;
}

void BorrowWidget::first()
{
    auto widget = currentWidget();
    if(widget)  widget->first();
}
void BorrowWidget::last()
{
    auto widget = currentWidget();
    if(widget)  widget->last();
}
void BorrowWidget::next()
{
    auto widget = currentWidget();
    if(widget)  widget->next();
}
void BorrowWidget::prev()
{
    auto widget = currentWidget();
    if(widget)  widget->prev();
}

//for BorrowAdmin Reader
void BorrowWidget::setStatusFor(WidgetStatus status)
{
    if(status == BorrowAdmin)
    {
        ui->borrow->setStatusFor(BorrowAdmin);
        ui->book->setStatusFor(BorrowAdmin);
        return;
    }

    ui->borrow->setStatusFor(Reader);

    auto name = record.value("rdID").toString();
    auto error = AbModel::seachReader(name,record);
    if(error.type() != QSqlError::NoError)
    {
        record = QSqlRecord();
        emit statusMes(U8("找不到此人"),3000);
        return;
    }

    ui->reader->readRecord(record);
    ui->borrow->setReader(name);

    ui->group_find->hide();
    ui->book->setStatusFor(Reader);
}

void BorrowWidget::on_btn_find_clicked()
{
    auto name = ui->edit_id->text();
    auto error = AbModel::seachReader(name,record);
    if(error.type() != QSqlError::NoError)
    {
        record = QSqlRecord();
        emit statusMes(U8("找不到此人"),3000);
        return;
    }

    ui->reader->readRecord(record);
    ui->borrow->setReader(name);
}

void BorrowWidget::BorrowBook(long long book)
{
    if(!record.isEmpty())
    {
        QString name = record.value(0).toString();
        auto error = AbModel::borrowBookProc(name, book);
        if(error.type() != QSqlError::NoError)
            showError(error.text());


        ui->book->modelPtr()->select();
        ui->borrow->setReader(name);
        error = AbModel::seachReader(name,record);
        qDebug()<<error.text()<<error.isValid();
        qDebug()<<error.type();
        if(error.type() != QSqlError::NoError)
        {
                showError(error.text());
        }

        ui->reader->readRecord(record);
    }
}

void BorrowWidget::ReturnBook(QItemSelectionModel* selection)
{
    if(!record.isEmpty())
    {
        auto ret = static_cast<BorrowInfoModel*>(modelPtr());

        if(ret->returnBook(selection))
        {
            ui->book->modelPtr()->select();//更新书

            QString name = record.value(0).toString();
            ui->borrow->setReader(name);//更新借阅

            auto error = AbModel::seachReader(name,record);
            if(error.type() != QSqlError::NoError)
                    showError(error.text());

            ui->reader->readRecord(record);//更新读者
        }
    }
}

Widget* BorrowWidget::currentWidget()
{
    Widget* widget = nullptr;
    widget = static_cast<Widget*>(ui->tabWidget->currentWidget());
    return widget;
}

QTableView* BorrowWidget::viewPtr()
{
    return currentWidget()->viewPtr();

}
AbModel* BorrowWidget::modelPtr()
{
    return currentWidget()->modelPtr();
}
