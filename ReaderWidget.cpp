#include "ReaderWidget.h"
#include "ReaderInfo.h"
#include "ui_ReaderWidget.h"
#include "Database.h"
#include "LoginDialog.h"
#include "MainWindow.h"
#include "ReaderDelegate.h"
#include <QDataWidgetMapper>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlRelationalDelegate>
#include <QSqlQuery>
#include <QItemSelectionModel>
#include <ctime>

ReaderWidget::ReaderWidget(QWidget *parent) :
    Widget(parent),
    ui(new Ui::ReaderWidget)
{
    ui->setupUi(this);

    initModel();
    initView();
    view = ui->tableView;
    tabModel = model;

    connect(ui->info,SIGNAL(new_data(QSqlRecord&)),this,SLOT(createItem(QSqlRecord&)));

    connect(ui->cmb_type,SIGNAL(currentIndexChanged(int)),this,SLOT(typeIndexChanged(int)));
}

void ReaderWidget::initView()
{
    ui->tableView->setModel(model);

    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //隐藏数据
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnHidden(8,true);
    ui->tableView->setColumnHidden(9,true);

    //初始化读者类别
    auto type = model->relationModel(4);
    //qDebug()<<model->fieldIndex("rdType");无法获取正确数据？？？
    //ui->info->setTypeModel(type, type->fieldIndex("typeName"));
    ui->cmb_type->setModel(type);
    ui->cmb_type->setModelColumn(1);

    //设置允许排序和行列自适应内容
    ui->tableView->setSortingEnabled(true);
//    ui->tableView->resizeColumnsToContents();
//    ui->tableView->resizeRowsToContents();
//    ui->tableView->setColumnWidth(3,ui->tableView->columnWidth(3)+10);
//    ui->tableView->setColumnWidth(8,ui->tableView->columnWidth(8)+20);
    ui->tableView->setCornerButtonEnabled(true);

    ui->tableView->setColumnWidth(0,100);
    ui->tableView->setColumnWidth(1,100);
    ui->tableView->setColumnWidth(2,100);
    ui->tableView->setColumnWidth(3,60);
    ui->tableView->setColumnWidth(4,100);//rdType
    ui->tableView->setColumnWidth(5,100);
    ui->tableView->setColumnWidth(6,80);
    ui->tableView->setColumnWidth(7,150);
    ui->tableView->setColumnWidth(8,120);//rdDate
    ui->tableView->setColumnWidth(9,80);//rdStatus
    ui->tableView->setColumnWidth(10,60);
    ui->tableView->setColumnWidth(11,120);
    ui->tableView->setColumnWidth(12,150);



    ui->tableView->setItemDelegate(new ReaderDelegate(ui->tableView));
}

void ReaderWidget::initModel()
{
    auto db = QSqlDatabase::database("Library");

    model = new QSqlRelationalTableModel(ui->tableView,db);
    model->setTable("Reader");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);


    model->setHeaderData(0,Qt::Horizontal,U8("读者编号"));
    model->setHeaderData(1,Qt::Horizontal,U8("密码"));
    model->setHeaderData(2,Qt::Horizontal,U8("姓名"));
    model->setHeaderData(3,Qt::Horizontal,U8("性别"));
    model->setHeaderData(4,Qt::Horizontal,U8("读者类型"));
    model->setHeaderData(5,Qt::Horizontal,U8("编号"));
    model->setHeaderData(6,Qt::Horizontal,U8("借书数量"));
    model->setHeaderData(7,Qt::Horizontal,U8("部门"));
    model->setHeaderData(8,Qt::Horizontal,U8("办证日期"));
    model->setHeaderData(9,Qt::Horizontal,U8("照片"));
    model->setHeaderData(10,Qt::Horizontal,U8("读者状态"));
    model->setHeaderData(11,Qt::Horizontal,U8("电话"));
    model->setHeaderData(12,Qt::Horizontal,U8("邮箱"));

    //设置关系外键
    model->setRelation(model->fieldIndex("rdType"),QSqlRelation("ReaderType","rdType","typeName"));

    //映射数据到info
    QDataWidgetMapper* mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setItemDelegate(new QItemDelegate(mapper));
    ui->info->setWidgetMapper(mapper);
    connect(ui->tableView,SIGNAL(clicked(const QModelIndex&)),
            mapper,SLOT(setCurrentModelIndex(const QModelIndex&)));

    if(!model->select())
        showError("获取数据出错，请联系数据库管理员");
}

ReaderWidget::~ReaderWidget()
{
    delete ui;
}

void ReaderWidget::closeEvent(QCloseEvent* event)
{
    qDebug()<<"ReaderWidget::closeEvent";
}

bool ReaderWidget::setRecord(const QSqlRecord& rec)
{
    bool ret = Widget::setRecord(rec);

    //设置read_info的Record，用于保存read的字段信息
    ui->info->readRecord(this->record);

    return ret;
}

void  ReaderWidget::newItem(bool checked)
{
    if(checked)
    {
        ui->info->setStatusFor(Create);
        ui->info->clear();
        ui->tableView->setEnabled(false);
        return;
    }

    ui->info->setStatusFor(Display);
    ui->tableView->setEnabled(true);
}

void  ReaderWidget::changeItem(bool checked)
{
    if(checked)
    {
        model->setEditStrategy(QSqlTableModel::OnFieldChange);
        ui->info->setStatusFor(Alter);
        return;
    }
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->info->setStatusFor(Display);
}

//void  ReaderWidget::deleteItem()
//{
//    auto list = ui->tableView->selectionModel()->selectedRows(0);
//    for(auto& i : list)
//        model->removeRow(i.row());

//    submitData();
//}

//void ReaderWidget::submitItem()
//{
//    submitData();
//}

void ReaderWidget::changePwd()
{
    qDebug()<<"changePwd()";
}

//void ReaderWidget::createItem(QSqlRecord& rec)
//{
//    if(!rec.isEmpty())
//    {
//        if(model->insertRecord(0,rec))
//        {
//            if(!model->submitAll())
//            {
//                model->revertAll();
//                showError(lastError());
//            }
//        }
//        else
//        {
//            showError(lastError());
//        }
//    }
//}

void ReaderWidget::on_btn_find_clicked()
{
    bool flag = ui->ckb_find->isChecked();
    QString str = flag ? "like" : "=";

    QString filter("1=1 ");

    auto text = ui->edit_id->text();
    if(text != "")
        filter = QString("rdID %1 '%2' ").arg(str).arg(flag ? "%"+text+"%" : text);

    text = ui->edit_name->text();
    if(text != "")
        filter += QString("and rdName %1 '%2' ").arg(str).arg(flag ? "%"+text+"%" : text);

    text = ui->edit_dept->text();
    if(text != "")
        filter += QString("and dept %1 '%2' ").arg(str).arg(flag ? "%"+text+"%" : text);

    if(filter == "1=1 ") filter = "";

    model->setFilter(filter);

    if(!model->select())
        showError(lastError());
}

void ReaderWidget::typeIndexChanged(int index)
{
    model->setFilter(QString("Reader.rdType = '%1' ").arg(index+1));
    if(!model->select())
        showError(lastError());
}

void ReaderWidget::on_btn_loss_clicked()
{
    setReaderStatus(1);

//    auto a = ui->tableView->selectionModel();
//    qDebug()<<a->selectedIndexes();
//    qDebug()<<a->selection();
//    //ui->tableView->setSelection()
//    qDebug()<<a->selectedRows(0);
}

void ReaderWidget::on_btn_user_disable_clicked()
{
    setReaderStatus(2);
}

void ReaderWidget::on_btn_normal_clicked()
{
    setReaderStatus(0);
}

void ReaderWidget::setReaderStatus(int status)
{
    auto list = ui->tableView->selectionModel()->selectedRows(model->fieldIndex("rdStatus"));
    for(auto& i : list)
        model->setData(i ,status);

    submitData();
}

//void ReaderWidget::submitData()
//{
//    if( !(model->submitAll() && model->select()) )
//        showError(lastError());
//}

//QString ReaderWidget::lastError()
//{
//    return model->lastError().text();
//}




