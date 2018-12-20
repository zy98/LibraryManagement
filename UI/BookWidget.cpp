#include "BookWidget.h"
#include "ui_BookWidget.h"

#include <QSqlError>
#include <QDebug>
#include <QItemDelegate>
#include <ctime>
#include <QAction>

#include <Delegate/BookDelegate.h>
#include <Delegate/BookWidgetMappingDelegate.h>


BookWidget::BookWidget(QWidget *parent) :
    Widget(parent),
    ui(new Ui::BookWidget),
    bookInfo(this)
{
    ui->setupUi(this);

    initModel();

    connect(&bookInfo,SIGNAL(prev()),this,SLOT(prev()));
    connect(&bookInfo,SIGNAL(next()),this,SLOT(next()));
    connect(&bookInfo,SIGNAL(new_data(QSqlRecord&)),this,SLOT(createItem(QSqlRecord&)));
    connect(&bookInfo,SIGNAL(updatePicture(QSharedPointer<QByteArray>)),
             this,SLOT(updatePicture(QSharedPointer<QByteArray>)));
}

BookWidget::~BookWidget()
{
    delete ui;
}

bool BookWidget::setRecord(const QSqlRecord& rec)
{
    bool ret = Widget::setRecord(rec);
    bookInfo.readRecord(model->record());
    return ret;
}

void BookWidget::initView()
{
    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(new BookDelegate(this));
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView->setSortingEnabled(true);

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
    auto db = QSqlDatabase::database("Library");
    model = new BookModel(ui->tableView,db);
}

void BookWidget::PrivateInitModel(WidgetStatus status)
{
    if(status == BookAdmin)
    {
        ui->groupBorrow->hide();
        model->setFetchTable("Book");

        //映射数据
        QDataWidgetMapper* mapper = new QDataWidgetMapper(this);
        mapper->setModel(model);
        mapper->setItemDelegate(new BookWidgetMappingDelegate(mapper));
        mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
        bookInfo.setWidgetMapper(mapper);

        connect(ui->tableView,SIGNAL(clicked(const QModelIndex&)),
                mapper,SLOT(setCurrentModelIndex(const QModelIndex&)));
    }
    else if(status == BorrowAdmin)
    {
        ui->groupBorrow->show();
        model->setFetchTable("bookInLibrary");
    }
    else
    {
        ui->groupBorrow->hide();
        model->setFetchTable("Book");
    }

    model->setManulSubmit();
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

    if(!model->select())
        showError(model->dbError());
}

//for BookAdmin BorrowAdmin Reader
void BookWidget::setStatusFor(WidgetStatus status)
{
    if(status == BookAdmin)
    {
        PrivateInitModel(BookAdmin);
        initView();
        ui->tableView->setEditTriggers(QTableView::DoubleClicked);
        ui->tableView->setSelectionMode(QTableView::ExtendedSelection);
        setColumnsHideFor(BookAdmin);

        return;
    }
    else if(status == BorrowAdmin)
    {
        PrivateInitModel(BorrowAdmin);
        initView();
        setColumnsHideFor(BorrowAdmin);
    }
    else
    {
        PrivateInitModel(Reader);
        initView();
        setColumnsHideFor(Reader);
    }

    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
    ui->tableView->setSelectionMode(QTableView::SingleSelection);
}

void BookWidget::newItem()
{
    model->select();
    bookInfo.setStatusFor(Create);
    bookInfo.clear();
    model->insertItem();
    ui->tableView->selectRow(0);
        emit ui->tableView->clicked(modelPtr()->index(0,0));
    bookInfo.show();
}

void BookWidget::changeItem()
{
    model->setAutoSubmit();
    bookInfo.setStatusFor(Alter);
    bookInfo.show();
}

bool BookWidget::createItem(QSqlRecord& rec)
{
    model->select();
    rec.remove(0);
    bool ret = modelPtr()->insertRecord(0,rec);

    if(ret)
    {
        if(!model->submitAll())
            showError(model->dbError());
        bookInfo.clear();
    }
    else
    {
        model->revertAll();
    }
    return ret;
}

//可以做一个map映射
void BookWidget::on_bkBtnFind_clicked()
{
    QMap<QString, QVariant> map;

    auto text = ui->edit_id->text();
    if(text != "")
        map["bkID"] = text;

    text = ui->edit_name->text();
    if(text != "")
        map["bkName"] = text;

    text = ui->edit_author->text();
    if(text != "")
        map["bkAuthor"] = text;

    text = ui->edit_pub->text();
    if(text != "")
        map["bkPub"] = text;

    if(!model->selectItem(map,ui->ckb_find->isChecked()))
        showError(model->dbError());
}

/////////////////////////////////////////////
void BookWidget::on_btn_borrow_clicked()
{
    auto list = ui->tableView->selectionModel()->selectedRows(0);
    if(list.size() == 1)
        emit borrowBook(list.at(0).data().toLongLong());//传递书的ID
    else
        emit statusMes(U8("请选择一项，且只能选择一项"),3000);
}

void BookWidget::setColumnsHideFor(WidgetStatus status)
{
    if(status == BorrowAdmin)
    {
        for(int i = 0; i < model->columnCount();i++)
            if( i >= 6 && i <= 12)
                viewPtr()->setColumnHidden(i, true);
            else
                viewPtr()->setColumnHidden(i,false);

        return;
    }
    for(int i = 0; i < model->columnCount();i++)
        if( i == 4 || i == 11 || i == 12)
            viewPtr()->setColumnHidden(i, true);
        else
            viewPtr()->setColumnHidden(i,false);
}

QTableView* BookWidget::viewPtr()
{
    return ui->tableView;
}
AbModel* BookWidget::modelPtr()
{
    return model;
}
