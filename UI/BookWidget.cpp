#include "BookWidget.h"
#include "ui_BookWidget.h"

#include <QSqlError>
#include <QDebug>
#include <QItemDelegate>
#include <ctime>


BookWidget::BookWidget(QWidget *parent) :
    Widget(parent),
    ui(new Ui::BookWidget)
{
    ui->setupUi(this);
    bookInfo.setWindowFlags(Qt::Window);
    ui->groupMatch->hide();

    auto db = QSqlDatabase::database("Library");
    model = new BookModel(ui->tableView,db);
    model->setFetchTable("Book");
    //initModel();
    initView();


    connect(&bookInfo,SIGNAL(prev()),this,SLOT(prev()));
    connect(&bookInfo,SIGNAL(next()),this,SLOT(next()));
    connect(&bookInfo,SIGNAL(new_data(QSqlRecord&)),this,SLOT(createItem(QSqlRecord&)));
}

BookWidget::~BookWidget()
{
    bookInfo.hide();
    bookInfo.close();
    qDebug()<<"BookWidget::~BookWidget()";

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
    //映射数据
    QDataWidgetMapper* mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setItemDelegate(new QItemDelegate(mapper));
    bookInfo.setWidgetMapper(mapper);
    connect(ui->tableView,SIGNAL(clicked(const QModelIndex&)),
            mapper,SLOT(setCurrentModelIndex(const QModelIndex&)));

    //select 并不耗时，但最好还是不要初始化时直接select
    //select时间和数据大小成正比
    if(!model->fetch())
        showError(model->dbError());
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
        //ui->groupMatch->show();
        setColumnsHideFor(BookAdmin);
        model->setFetchTable("Book");
        initModel();
        model->setAutoSubmit();
        return;
    }
    else if(status == BorrowAdmin)
    {
        ui->groupBorrow->show();
        ui->groupMatch->hide();
        model->setFetchTable("bookInLibrary");
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
        bookInfo.clear();
        bookInfo.setStatusFor(Create);
        bookInfo.show();
        return;
    }
    bookInfo.setStatusFor(Display);
    bookInfo.hide();
}

void BookWidget::changeItem(bool checked)
{
    if(checked)
    {
        model->setAutoSubmit();
        bookInfo.show();
        bookInfo.setStatusFor(Alter);
        return;
    }
    model->setManulSubmit();
    bookInfo.hide();
    bookInfo.setStatusFor(Display);
}

bool BookWidget::createItem(QSqlRecord& rec)
{
    bool ret = Widget::createItem(rec);

    if(ret && model->submitAll())
    {
        bookInfo.clear();
        model->insertItem();
    }
    if(ret) model->revertAll();

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
        emit borrowBook(list.at(0).data().toLongLong());
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

QTableView* BookWidget::viewPtr()
{
    return ui->tableView;
}
AbModel* BookWidget::modelPtr()
{
    return model;
}
