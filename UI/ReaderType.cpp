#include "ReaderType.h"
#include "ui_ReaderType.h"

#include <QContextMenuEvent>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QMenu>
#include <QMessageBox>
#include <QSqlError>

ReaderType::ReaderType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReaderType),
    menu(new QMenu(this))
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::database("Library");
    model = new QSqlRelationalTableModel(this,db);
    model->setTable("ReaderType");
    model->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
    ui->tableView->setSelectionMode(QTableView::ExtendedSelection);

    model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("类型编号"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("类型名称"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("最大数量"));
    model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("最长天数"));
    model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("续借次数"));
    model->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("账户有效期"));

    menu->addAction(ui->actionNew);
    menu->addAction(ui->actionRemove);
    menu->addAction(ui->actionSubmit);

}

void ReaderType::contextMenuEvent(QContextMenuEvent *event)
{
    menu->exec(event->globalPos());
}

ReaderType::~ReaderType()
{
    delete ui;
}

void ReaderType::on_actionNew_triggered()
{
    model->insertRow(model->rowCount());
    ui->tableView->selectRow(model->rowCount() - 1);
    emit ui->tableView->clicked(model->index(model->rowCount() - 1,0));
}

void ReaderType::on_actionRemove_triggered()
{
    auto index = ui->tableView->selectionModel()->selectedRows(0);
    for(auto& i : index)
        model->removeRow(i.row());

    model->select();
}

void ReaderType::on_actionSubmit_triggered()
{
    if(!model->submitAll())
        QMessageBox::critical(this,"Error",model->lastError().text());
    model->select();
}
