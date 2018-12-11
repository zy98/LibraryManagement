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
}

BorrowWidget::~BorrowWidget()
{
    delete ui;
}

void BorrowWidget::first()
{
    auto widget = static_cast<Widget*>(ui->tabWidget->currentWidget());
    if(widget)
        widget->first();
}
void BorrowWidget::last()
{
    auto widget = static_cast<Widget*>(ui->tabWidget->currentWidget());
    if(widget)
        widget->last();
}
void BorrowWidget::next()
{
    auto widget = static_cast<Widget*>(ui->tabWidget->currentWidget());
    if(widget)
        widget->next();
}
void BorrowWidget::prev()
{
    auto widget = static_cast<Widget*>(ui->tabWidget->currentWidget());
    if(widget)
        widget->prev();
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
    ui->book->setStatusFor(Reader);
}

void BorrowWidget::on_btn_find_clicked()
{
    readerID = "";
    auto id = ui->edit_id->text();
    if(id == "")
        return;

    QSqlDatabase db = QSqlDatabase::database("Library");

    QSqlQuery query(db);   
    query.prepare("select * from Reader where rdID = ?");
    query.addBindValue(id);
    if(!query.exec())
    {
        showError(query.lastError().databaseText());
        return;
    }

    if(!query.first())
    {
        emit statusMes(U8("找不到读者：") + id, 3000);
        return;
    }
    auto rec = query.record();
    ui->reader->readRecord(rec);
    readerID = rec.value(0).toString();
    ui->borrow->setReader(readerID);
    qDebug()<<"find";
}

void BorrowWidget::setCheck(QString rdid, long long bkid)
{

}

void BorrowWidget::BorrowBook(long long book)
{
    qDebug()<<"Borrow:"<<book;
    if(readerID == "")
        return;

    QSqlDatabase db = QSqlDatabase::database("Library");

    QSqlQuery query(db);
    query.prepare("exec usp_borrow_book ? , ?");
    qDebug()<<"reader:"<<readerID<<"  book:"<<book;
    query.addBindValue(readerID);
    query.addBindValue(book);

    if(!query.exec())
    {
        showError(query.lastError().databaseText());
        return;
    }
}
