#include "AbModel.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRelationalTableModel>

AbModel::AbModel(QObject *parent, QSqlDatabase db):
    QSqlRelationalTableModel(parent, db)
{

}

AbModel::~AbModel(){}

bool AbModel::seachReader(const QString& name,QSqlRecord& rec)
{
    bool ret = !name.isEmpty();
    if(ret)
    {
        QSqlDatabase db = QSqlDatabase::database("Library");
        QSqlQuery query(db);
        query.prepare("select * from Reader where rdID = ?");
        query.addBindValue(name);
        if(query.exec() && query.first())
            rec = query.record();
        else
            ret = false;
    }
    return ret;
}

bool AbModel::borrowBookProc(const QString& rdid, long long bkid)
{
    bool ret = true;
    QSqlDatabase db = QSqlDatabase::database("Library");

    int retCode = 0;
    QSqlQuery query(db);
    query.prepare(" ? = exec usp_borrow_book ? , ? ");
    query.addBindValue(retCode,QSql::Out);
    query.addBindValue(rdid);
    query.addBindValue(bkid);

    if( !query.exec() || retCode == 0 )
        ret = false;

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
    return submitAll() && select();
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
            if(ret) break;
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

        qDebug()<<"filter:"<<filterStr;
       setFilter(filterStr);
    }
    ret = select();
    qDebug()<<"selectItem:"<<selectStatement() <<" "<<ret;
    return ret;
}

