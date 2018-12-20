#include "BookModel.h"

#include <QSharedPointer>
#include <QSqlQuery>

BookModel::BookModel(QObject *parent, QSqlDatabase db):
    AbModel(parent,db)
{

}

bool BookModel::fillRowItem(int row, QSqlRecord &rec)
{
    bool ret = true;
    for(int i = 1; i < rec.count(); i++)
        ret = setData(index(row,i), rec.value(i));
    return ret;
}

bool BookModel::uploadPicture
(QItemSelectionModel* selection, QSharedPointer<QByteArray> dataImg)
{
    bool ret = true;
    auto list = selection->selectedRows(12);
    if(list.size() == 1)
        ret = setData(list[0],*dataImg);

    return ret && submitData();
}
