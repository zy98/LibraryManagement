#include "BorrowInfoWidget.h"
#include "ui_BorrowInfoWidget.h"

#include "Model/BorrowInfoModel.h"

#include <QSqlRelationalTableModel>
#include <QDebug>
#include <QButtonGroup>

BorrowInfoWidget::BorrowInfoWidget(QWidget *parent) :
    Widget(parent),
    ui(new Ui::BorrowInfoWidget)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::database("Library");
    model = new BorrowInfoModel(ui->tableView,db);

    QButtonGroup* btn_group = new QButtonGroup(this);
    btn_group->addButton(ui->chk_all,0);
    btn_group->addButton(ui->chk_noreturn,1);
    btn_group->addButton(ui->chk_relend,2);
    btn_group->addButton(ui->chk_over,3);


    initModel();
    initView();

    connect(btn_group,SIGNAL(buttonClicked(int)),this,SLOT(BtnFilter(int)));
}

BorrowInfoWidget::~BorrowInfoWidget()
{
    delete ui;
}

//for BorrowAdmin Reader
void BorrowInfoWidget::setStatusFor(WidgetStatus status)
{
    if(status == BorrowAdmin)
        ui->groupBorrow->show();
    else
        ui->groupBorrow->hide();
}

void BorrowInfoWidget::initView()
{
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
    ui->tableView->setSelectionMode(QTableView::SingleSelection);
    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);

    ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(4,true);
}
void BorrowInfoWidget::initModel()
{
}

void BorrowInfoWidget::BtnFilter(int id)
{
    if(id == 1)
    {
        model->setFilter("returnDate is NULL");
        model->select();
    }
    else if(id == 2)
    {
        //实际应该与读者类型表比较
        model->setFilter("lendTimes = 0");
        model->select();
    }
    else if(id == 3)
    {
        model->setFilter("outDays != 0");
        model->select();
    }

    model->setFilter("");
    model->select();
}

void BorrowInfoWidget::setReader(const QString& rd)
{
    QMap<QString, QVariant> map;
    map["rdID"] = rd;
    if(!model->selectItem(map,false))
        showError(model->dbError());
}

QTableView* BorrowInfoWidget::viewPtr()
{
    return ui->tableView;
}
AbModel* BorrowInfoWidget::modelPtr()
{
    return model;
}


void BorrowInfoWidget::on_btn_returnBook_clicked()
{
    emit returnBook(ui->tableView->selectionModel());
}
