#include "AbModel.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRelationalTableModel>

AbModel::AbModel(QObject *parent, QSqlDatabase db):
    QSqlRelationalTableModel(parent, db)
{

}

AbModel::~AbModel(){}

QSqlError AbModel::seachReader(const QString& name,QSqlRecord& rec)
{
    QSqlError ret;
    qDebug()<<ret;
    if(!name.isEmpty())
    {
        QSqlDatabase db = QSqlDatabase::database("Library");
        QSqlQuery query(db);
        query.prepare("select * from Reader where rdID = ?");
        query.addBindValue(name);
        if(query.exec() && query.first())
            rec = query.record();
        else
            ret = db.lastError();
    }
    return ret;
}

QSqlError AbModel::borrowBookProc(const QString& rdid, long long bkid)
{
    QSqlError ret;
    QSqlDatabase db = QSqlDatabase::database("Library");

    QSqlQuery query(db);
    query.prepare("exec usp_borrow_book ? , ? ");
    query.addBindValue(rdid);
    query.addBindValue(bkid);

    if( !query.exec())
        ret = db.lastError();

    return ret;
}

QString AbModel::U8(const char* s)
{
    return QString::fromLocal8Bit(s);
}

void AbModel::setFetchTable(const QString& name)
{
    setTable(name);
}
void AbModel::setManulSubmit()
{
    setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
}
void AbModel::setAutoSubmit()
{
    setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
}

bool AbModel::fetch()
{
    return select();
}
bool AbModel::submitData()
{
    bool ret = submitAll();
    if(!ret)
        revert();

    return ret;
    //return submitAll() && select();
}

QString AbModel::dbError()
{
    return lastError().text();
}

bool AbModel::insertItem(int count)
{
    return insertRows(0,count);
}

bool AbModel::fillRowItem(int row, QSqlRecord& rec)
{
    bool ret = true;
    for(int i = 0; i < rec.count() && ret; i++)
        ret = setData(index(row,i),rec.value(i));
    return ret;
}

bool AbModel::deleteItem(QItemSelectionModel* selection)
{
    bool ret = selection != nullptr;
    if(ret)
    {
        QModelIndexList list = selection->selectedRows(0);
        for(auto& i : list)
        {
            ret = deleteRowFromTable(i.row());
            if(!ret) break;
        }
    }
    ret = submitData();

    return ret;
}

bool AbModel::selectItem(const QMap<QString, QVariant> &filter, bool flag)
{
    bool ret = !filter.isEmpty();
    setFilter("");
    if(ret)
    {
        QString token = flag ? " like '%%1%' " : " = '%1' ";

        auto i = filter.begin();
        QString filterStr = i.key() + token.arg(i.value().toString());
        for(++i; i != filter.end(); i++)
            filterStr += QString(" and ") + i.key() + token.arg(i.value().toString());

       setFilter(filterStr);
    }
    ret = select();
    return ret;
}

bool AbModel::uploadPicture
(QItemSelectionModel* selection, QSharedPointer<QByteArray> dataImg)
{
    bool ret = true;
    auto list = selection->selectedRows(9);
    if(list.size() == 1)
        ret = setData(list[0],*dataImg);

    ret = ret && submitData() && select();

    return ret;
}
