#include "BookWidget.h"
#include "ui_BookWidget.h"

#include <QSqlError>
#include <QDebug>
#include <QItemDelegate>
#include <ctime>

BookWidget::BookWidget(QWidget *parent) :
    Widget(parent),
    ui(new Ui::BookWidget),
    bookInfo(new BookInfo(this))
{
    ui->setupUi(this);
    bookInfo->setWindowFlags(Qt::Dialog);

    auto db = QSqlDatabase::database("Library");
    model = new QSqlTableModel(ui->tableView,db);
    model->setTable("Book");
    //initModel();
    initView();

    view = ui->tableView;
    tabModel = model;

    connect(bookInfo,SIGNAL(prev()),this,SLOT(prev()));
    connect(bookInfo,SIGNAL(next()),this,SLOT(next()));
    connect(bookInfo,SIGNAL(new_data(QSqlRecord&)),this,SLOT(createItem(QSqlRecord&)));
}

BookWidget::~BookWidget()
{
    delete ui;
}

bool BookWidget::setRecord(const QSqlRecord& rec)
{
    bool ret = Widget::setRecord(rec);
    bookInfo->readRecord(model->record());
    return ret;
}

void BookWidget::initView()
{
    ui->tableView->setModel(model);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置隐藏列
    setColumnsHideFor(BookAdmin);

    ui->tableView->setSortingEnabled(true);


//    由于自适应列宽占用大量时间，所以改为固定列宽；
//    ui->tableView->resizeColumnsToContents();
//    ui->tableView->resizeRowsToContents();

    //设置固定列宽
    ui->tableView->setColumnWidth(0,100);
    ui->tableView->setColumnWidth(1,300);
    ui->tableView->setColumnWidth(2,150);
    ui->tableView->setColumnWidth(3,200);
    ui->tableView->setColumnWidth(4,100);
    ui->tableView->setColumnWidth(5,150);
    ui->tableView->setColumnWidth(6,100);
    ui->tableView->setColumnWidth(7,80);
    ui->tableView->setColumnWidth(8,80);
    ui->tableView->setColumnWidth(9,80);
    ui->tableView->setColumnWidth(10,100);
    ui->tableView->setColumnWidth(11,300);
    ui->tableView->setColumnWidth(12,300);
    ui->tableView->setColumnWidth(13,80);
    ui->tableView->setColumnWidth(14,50);
}

void BookWidget::initModel()
{    
    model->setHeaderData(0,Qt::Horizontal,U8("图书编号"));
    model->setHeaderData(1,Qt::Horizontal,U8("书名"));
    model->setHeaderData(2,Qt::Horizontal,U8("作者"));
    model->setHeaderData(3,Qt::Horizontal,U8("出版社"));
    model->setHeaderData(4,Qt::Horizontal,U8("出版日期"));
    model->setHeaderData(5,Qt::Horizontal,U8("ISBN"));
    model->setHeaderData(6,Qt::Horizontal,U8("分类"));
    model->setHeaderData(7,Qt::Horizontal,U8("语言"));
    model->setHeaderData(8,Qt::Horizontal,U8("页数"));   
    model->setHeaderData(9,Qt::Horizontal,U8("价格"));
    model->setHeaderData(10,Qt::Horizontal,U8("入库日期"));
    model->setHeaderData(11,Qt::Horizontal,U8("简介"));
    model->setHeaderData(12,Qt::Horizontal,U8("封面"));
    model->setHeaderData(13,Qt::Horizontal,U8("状态"));
    model->setHeaderData(14,Qt::Horizontal,U8("数量"));

    //映射数据
    QDataWidgetMapper* mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setItemDelegate(new QItemDelegate(mapper));
    bookInfo->setWidgetMapper(mapper);
    connect(ui->tableView,SIGNAL(clicked(const QModelIndex&)),
            mapper,SLOT(setCurrentModelIndex(const QModelIndex&)));

    //select 并不耗时，但最好还是不要初始化时直接select
    //select时间和数据大小成正比
    if(!model->select())
        showError(lastError());
}

//void BookWidget::closeEvent(QCloseEvent* event)
//{
//    qDebug()<<"BookWidget::closeEvent(QCloseEvent* event)";
//}

//for BookAdmin BorrowAdmin Reader
void BookWidget::setStatusFor(WidgetStatus status)
{
    if(status == BookAdmin)
    {
        ui->groupBorrow->hide();
        ui->groupMatch->show();
        setColumnsHideFor(BookAdmin);
        model->setTable("Book");
        initModel();
        model->setEditStrategy(QSqlTableModel::OnFieldChange);
        return;
    }
    else if(status == BorrowAdmin)
    {
        ui->groupBorrow->show();
        ui->groupMatch->hide();
        model->setTable("bookInLibrary");
        initModel();
        ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
        ui->tableView->setSelectionMode(QTableView::SingleSelection);
        setColumnsHideFor(BorrowAdmin);

        return;
    }

    ui->groupBorrow->hide();
    ui->groupMatch->hide();
    setColumnsHideFor(BookAdmin);
}

void BookWidget::newItem(bool checked)
{
    if(checked)
    {
        bookInfo->clear();

        bookInfo->setStatusFor(Create);
        bookInfo->show();
    }
    else
    {
        bookInfo->setStatusFor(Display);
        bookInfo->hide();
    }

}

void BookWidget::changeItem(bool checked)
{
    if(checked)
    {
        model->setEditStrategy(QSqlTableModel::OnFieldChange);
        bookInfo->show();
        bookInfo->setStatusFor(Alter);
        return;
    }
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    bookInfo->hide();
    bookInfo->setStatusFor(Display);
}
//void BookWidget::submitItem(){}
void BookWidget::changePwd(){}

void BookWidget::createItem(QSqlRecord& rec)
{
    if(!rec.isEmpty())
    {
        bool ok = false;
        auto num = rec.value(14).toInt(&ok);
        if(!ok)
        {
            showError("转换书籍数量出错");
            return;
        }

        qDebug()<<"num"<<num;
        rec.remove(0);
        for(int i = 0; i < num; i++)
            if(!model->insertRecord(-1,rec))
            {
                showError(lastError());
            }

        if(!model->submitAll())
        {
            qDebug()<<"!model->submitAll()";
            model->revertAll();
            showError(lastError());
        }
    }
}

//可以做一个map映射
void BookWidget::on_bkBtnFind_clicked()
{
    bool flag = ui->ckb_find->isChecked();
    QString str = flag ? "like" : "=";

    QString filter("1=1 ");

    auto text = ui->edit_id->text();
    if(text != "")
        filter = QString("bkID %1 '%2' ").arg(str).arg(flag ? "%"+text+"%" : text);

    text = ui->edit_name->text();
    if(text != "")
        filter += QString("and bkName %1 '%2' ").arg(str).arg(flag ? "%"+text+"%" : text);

    text = ui->edit_author->text();
    if(text != "")
        filter += QString("and bkAuthor %1 '%2' ").arg(str).arg(flag ? "%"+text+"%" : text);

    text = ui->edit_pub->text();
    if(text != "")
        filter += QString("and bkPub %1 '%2' ").arg(str).arg(flag ? "%"+text+"%" : text);

    if(filter == "1=1 ") filter = "";

    model->setFilter(filter);

    if(!model->select())
        showError(lastError());
}

void BookWidget::on_btn_detail_clicked(bool checked)
{
    bookInfo->setStatusFor(Display);
    checked ? bookInfo->show() : bookInfo->hide();
}

void BookWidget::on_btn_borrow_clicked()
{
    auto list = ui->tableView->selectionModel()->selectedRows(0);
    if(list.size() == 1)
    {
        qDebug()<<"emit:"<<list.at(0).data().toLongLong();
        emit borrowBook(list.at(0).data().toLongLong());
    }
    else
        emit statusMes(U8("请选择一项，且只能选择一项"),3000);
}

void BookWidget::setColumnsHideFor(WidgetStatus status)
{
    if(status == BorrowAdmin)
    {
        for(int i = 0; i < model->columnCount();i++)
            if( i >= 6 && i <= 12)
                ui->tableView->setColumnHidden(i, true);
            else
                ui->tableView->setColumnHidden(i,false);
    }
    else
    {
        for(int i = 0; i < model->columnCount();i++)
            if( i == 4 || i ==11 || i == 12)
                ui->tableView->setColumnHidden(i, true);
            else
                ui->tableView->setColumnHidden(i,false);
    }
}
