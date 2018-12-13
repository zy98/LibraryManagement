#include "BorrowInfoWidget.h"
#include "ui_BorrowInfoWidget.h"

#include "Model/BorrowInfoModel.h"

#include <QSqlRelationalTableModel>
#include <QDebug>

BorrowInfoWidget::BorrowInfoWidget(QWidget *parent) :
    Widget(parent),
    ui(new Ui::BorrowInfoWidget)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::database("Library");
    model = new BorrowInfoModel(ui->tableView,db);

    initModel();
    initView();
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
}
void BorrowInfoWidget::initModel()
{
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
    if(!model->returnBook(ui->tableView->selectionModel()))
        showError(model->dbError());
}
