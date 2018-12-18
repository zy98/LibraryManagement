#include "UI/ReaderWidget.h"
#include "UI/ReaderInfo.h"
#include "ui_ReaderWidget.h"
#include "MainWindow.h"
#include "ReaderType.h"
#include "Delegate//ReaderDelegate.h"
#include <QDataWidgetMapper>
#include <QDebug>
#include <QSqlError>
#include <QSqlRelationalDelegate>
#include <QSqlQuery>
#include <QInputDialog>
#include <ctime>
#include "Delegate/ReaderWidgetMappingDelegate.h"


ReaderWidget::ReaderWidget(QWidget *parent) :
    Widget(parent),
    ui(new Ui::ReaderWidget)
{
    ui->setupUi(this);

    initModel();
    initView();

    //设置读者类型model
    //ui->info->setReaderType(ui->cmb_type->model());

    connect(ui->info,SIGNAL(new_data(QSqlRecord&)),this,SLOT(createItem(QSqlRecord&)));
    connect(ui->cmb_type,SIGNAL(currentIndexChanged(int)),this,SLOT(typeIndexChanged(int)));
    connect(ui->info,SIGNAL(updatePicture(QSharedPointer<QByteArray>)),
            this,SLOT(updatePicture(QSharedPointer<QByteArray>)));
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
    ui->info->setTypeModel(type, type->fieldIndex("typeName"));
    ui->cmb_type->setModel(type);
    ui->cmb_type->setModelColumn(1);

    //设置允许排序和行列自适应内容
    ui->tableView->setSortingEnabled(true);
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
    ui->tableView->setColumnWidth(10,80);
    ui->tableView->setColumnWidth(11,120);
    ui->tableView->setColumnWidth(12,150);

    ui->tableView->setItemDelegate(new ReaderDelegate(ui->tableView));
}

void ReaderWidget::initModel()
{
    auto db = QSqlDatabase::database("Library");
    model = new ReaderModel(ui->tableView,db);

    //映射数据到info
    QDataWidgetMapper* mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setItemDelegate(new ReaderWidgetMappingDelegate(mapper));
    ui->info->setWidgetMapper(mapper);
    connect(ui->tableView,SIGNAL(clicked(const QModelIndex&)),
            mapper,SLOT(setCurrentModelIndex(const QModelIndex&)));

    if(!model->select())
        showError("获取数据出错，请联系数据库管理员");
    else
        emit statusMes(U8("获取数据成功"),3000);
}

ReaderWidget::~ReaderWidget()
{
    delete ui;
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
        qDebug()<<"newItem:"<<model->insertRow(0);
        ui->tableView->selectRow(0);
        emit ui->tableView->clicked(modelPtr()->index(0,0));
        //ui->tableView->setEnabled(false);
        return;
    }

    ui->info->setStatusFor(Display);
    ui->tableView->setEnabled(true);
    model->select();
}

void  ReaderWidget::changeItem(bool checked)
{
    if(checked)
    {
        model->setAutoSubmit();
        ui->info->setStatusFor(Alter);
        return;
    }
    model->setManulSubmit();
    ui->info->setStatusFor(Display);
    model->select();
}

//可以使用map
void ReaderWidget::on_btn_find_clicked()
{
    QMap<QString, QVariant> map;

    auto text = ui->edit_id->text();
    if(text != "")
        map["rdID"] = text;

    text = ui->edit_name->text();
    if(text != "")
        map["rdName"] = text;

    text = ui->edit_dept->text();
    if(text != "")
        map["rdDept"] = text;

    if(!model->selectItem(map,ui->ckb_find->isChecked()))
        showError(model->dbError());
}

void ReaderWidget::typeIndexChanged(int index)
{
    QMap<QString,QVariant> map;
    map["Reader.rdType"] = index + 1;
    if(!model->selectItem(map,false))
        showError(model->dbError());
}

void ReaderWidget::on_btn_loss_clicked()
{
    model->setRdStatusLoss(ui->tableView->selectionModel());
}

void ReaderWidget::on_btn_user_disable_clicked()
{
    model->setRdStatusDisabled(ui->tableView->selectionModel());
}

void ReaderWidget::on_btn_normal_clicked()
{
    model->setRdStatusNomal(ui->tableView->selectionModel());
}

QTableView* ReaderWidget::viewPtr()
{
    return ui->tableView;
}

AbModel* ReaderWidget::modelPtr()
{
    return model;
}

bool ReaderWidget::createItem(QSqlRecord &rec)
{
    bool ret = Widget::createItem(rec);

    if(ret && model->submitAll())
    {

        qDebug()<<"createItem:"<<model->insertRow(0);
        ui->tableView->selectRow(0);
        emit ui->tableView->clicked(model->index(0,0));
    }

    if(ret) model->revertAll();

    return ret;
}

void ReaderWidget::on_btn_reset_clicked()
{
    model->resetPassword(ui->tableView->selectionModel());
}

void ReaderWidget::on_btn_reapply_clicked()
{
    auto seModel = ui->tableView->selectionModel();
    if(seModel->selectedRows(0).size() == 1)
    {
        QSqlRecord rec;
        ui->info->writeRecord(rec);
        if(!rec.isEmpty())
        {
            QString input = QInputDialog::getText(this,U8("读者补办"),U8("请输入新ID:"));
            if(input != "")
            {
                rec.setValue(0,input);
                qDebug()<<rec;
                model->reapply(seModel, rec);
            }
        }
    }
    modelPtr()->select();
}

void ReaderWidget::on_btn_type_clicked()
{
    ReaderType typeDialog(this);
    typeDialog.exec();
}
