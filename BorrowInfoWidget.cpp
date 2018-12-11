#include "BorrowInfoWidget.h"
#include "ui_BorrowInfoWidget.h"

#include <QSqlRelationalTableModel>
#include <QDebug>

BorrowInfoWidget::BorrowInfoWidget(QWidget *parent) :
    Widget(parent),
    ui(new Ui::BorrowInfoWidget)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::database("Library");
    model = new QSqlRelationalTableModel(ui->tableView,db);
    model->setTable("Borrow");

    initModel();
    initView();

    tabModel = model;
    view = ui->tableView;
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
    ui->tableView->setSelectionMode(QTableView::ExtendedSelection);
}
void BorrowInfoWidget::initModel()
{
}

void BorrowInfoWidget::setReader(const QString& rd)
{
    model->setFilter(QString("rdID = %1 ").arg(rd));
    qDebug()<<"setFilter";
    if(!model->select())
    {
        qDebug()<<"select";
        showError(lastError());
    }
}

