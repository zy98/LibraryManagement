#include "Widget.h"
#include <QSqlError>
#include <QDebug>

Widget::Widget(QWidget *parent) : AbWidget(parent)
{
}

Widget::~Widget(){}

bool Widget::setRecord(const QSqlRecord& rec)
{
    record = rec;
    bool ret =true;
    if(record.isEmpty())
        ret = false;

    return ret;
}

void Widget::first()
{
    view->selectRow(0);
    emit view->clicked(tabModel->index(0,0));
}

void Widget::last()
{
    auto rowCount = tabModel->rowCount() - 1;
    view->selectRow(rowCount);
    emit view->clicked(tabModel->index(rowCount,0));
}

void Widget::next()
{
    auto row = view->currentIndex().row() + 1;
    auto rowCount = tabModel->rowCount() - 1;

    if(row > rowCount)
        row = 0;
    view->selectRow(row);
    emit view->clicked(tabModel->index(row,0));
}

void Widget::prev()
{
    auto row = view->currentIndex().row() - 1;
    auto rowCount = tabModel->rowCount() - 1;

    if(row < 0) row = rowCount;

    view->selectRow(row);
    emit view->clicked(tabModel->index(row,0));
}

void Widget::newItem(bool checked)
{

}
void Widget::changeItem(bool checked)
{

}
void Widget::deleteItem()
{
    auto list = view->selectionModel()->selectedRows(0);
    for(auto& i : list)
        tabModel->removeRow(i.row());

    submitData();
}

void Widget::submitItem()
{
    submitData();
}

void Widget::changePwd()
{

}

void Widget::createItem(QSqlRecord &rec)
{
    if(!rec.isEmpty())
    {
        if(!tabModel->insertRecord(0,rec))
            showError(lastError());

        if(!tabModel->submitAll())
        {
            tabModel->revertAll();
            showError(lastError());
        }
    }
}

void Widget::submitData()
{
    if( !(tabModel->submitAll() && tabModel->select()) )
        showError(lastError());
}

QString Widget::lastError()
{
    return tabModel->lastError().databaseText();
}

