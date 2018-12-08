#include "BookWidget.h"
#include "ui_BookWidget.h"

#include <QSqlError>
#include <QDebug>
#include <QItemDelegate>

BookWidget::BookWidget(QWidget *parent) :
    Widget(parent),
    ui(new Ui::BookWidget),
    bookInfo(new BookInfo(nullptr))
{
    ui->setupUi(this);
    initModel();
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

    ui->tableView->setSortingEnabled(true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    //设置隐藏列
    ui->tableView->setColumnHidden(4,true);
    ui->tableView->setColumnHidden(11,true);
    ui->tableView->setColumnHidden(12,true);

    //设置列宽



}

void BookWidget::initModel()
{
    auto db = QSqlDatabase::database("Library");

    model = new QSqlTableModel(ui->tableView,db);
    model->setTable("Book");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);

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


    if(!model->select())
        showError(model->lastError().text());
}

//void BookWidget::closeEvent(QCloseEvent* event)
//{
//    qDebug()<<"BookWidget::closeEvent(QCloseEvent* event)";
//}

void BookWidget::newItem(bool checked)
{
    if(checked)
    {
        bookInfo->clear();
        emit status_changed(1);
        bookInfo->show();
    }
    else
    {
        emit status_changed(0);
        bookInfo->hide();
    }
}

void BookWidget::changeItem(bool checked)
{
    model->setEditStrategy(checked ? QSqlTableModel::OnFieldChange : QSqlTableModel::OnManualSubmit);
    emit status_changed((checked ? 2 : 0));
}
//void BookWidget::submitItem(){}
void BookWidget::changePwd(){}

void BookWidget::createItem(QSqlRecord& rec)
{
    qDebug()<<"createItem";
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
