#include "ReaderModel.h"

#include <QModelIndex>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QInputDialog>


ReaderModel::ReaderModel(QObject *parent, QSqlDatabase db):
    AbModel(parent, db),
    rdStatus("rdStatus")
{
    setFetchTable("Reader");
    setManulSubmit();

    //bool setHeaderData(QStringList&);
    setHeaderData(0,Qt::Horizontal,U8("读者编号"));
    setHeaderData(1,Qt::Horizontal,U8("密码"));
    setHeaderData(2,Qt::Horizontal,U8("姓名"));
    setHeaderData(3,Qt::Horizontal,U8("性别"));
    setHeaderData(4,Qt::Horizontal,U8("读者类型"));
    setHeaderData(5,Qt::Horizontal,U8("编号"));
    setHeaderData(6,Qt::Horizontal,U8("借书数量"));
    setHeaderData(7,Qt::Horizontal,U8("部门"));
    setHeaderData(8,Qt::Horizontal,U8("办证日期"));
    setHeaderData(9,Qt::Horizontal,U8("照片"));
    setHeaderData(10,Qt::Horizontal,U8("读者状态"));
    setHeaderData(11,Qt::Horizontal,U8("电话"));
    setHeaderData(12,Qt::Horizontal,U8("邮箱"));

    //设置关系外键
    setRelation(fieldIndex("rdType"),QSqlRelation("ReaderType","rdType","typeName"));
}

QSqlError ReaderModel::setLoss(const QString& name)
{
    QSqlError ret;
    QSqlDatabase db = QSqlDatabase::database("Library");
    QSqlQuery query(db);
    query.prepare("update Reader set rdStatus = ? where rdID = ?");

    query.addBindValue(1);
    query.addBindValue(name);

    if(!query.exec())
        ret = db.lastError();

    return ret;
}

bool ReaderModel::setRdStatusLoss(QItemSelectionModel* selection)
{
    return setRdStatus(selection, 1);
}
bool ReaderModel::setRdStatusNomal(QItemSelectionModel* selection)
{  
    return setRdStatus(selection, 0);
}
bool ReaderModel::setRdStatusDisabled(QItemSelectionModel* selection)
{
    bool ret = (selection != nullptr);
    if(ret)
    {
        auto rdList = selection->selectedRows(0);
        auto statusList = selection->selectedRows(10);
        auto db = database();
        QSqlQuery query(db);
        for(int i = 0; i < rdList.size(); i++)
        {
            //从未借书的情况可以直接注销
            query.prepare("select * from Borrow where rdID = ? ");
            query.addBindValue(rdList[i].data().toString());
            ret = query.exec();

            if(ret && !query.next())
            {
                setData(statusList[i],2);
                continue;
            }

            //借书并且没有未还或欠款记录的可以注销
            query.prepare("select * from Borrow where rdID = '?' "
                          "and ( returnDate is NULL or fine > 0 )" );
            query.addBindValue(rdList[i].data().toString());
            ret = ret && query.exec();

            if( ret && query.first())
                setData(statusList[i], 2);
        }
    }
    submitData();
    return ret;
}

bool ReaderModel::setRdStatus(QItemSelectionModel* selection, int status)
{
    bool ret = (selection != nullptr);
    if(ret)
    {
        QModelIndexList indexList = selection->selectedRows(this->fieldIndex(rdStatus));
        for(auto& i : indexList)
        {
            ret = setData(i, status);
            if(!ret) break;
        }
    }
    ret = submitData();
    return ret;
}

bool ReaderModel::resetPassword(QItemSelectionModel* selection)
{
    bool ret = selection != nullptr;
    if(ret)
    {
        auto list = selection->selectedRows(1);
        for(auto& i : list)
        {
            ret = setData(i,"123456");
            if(!ret) break;
        }
    }
    ret = ret && submitData();

    return ret;
}

bool ReaderModel::reapply(QItemSelectionModel* selection,QSqlRecord& rec)
{
    bool ret = true;
    auto status = selection->selectedRows(10);
    auto id  = selection->selectedRows(0);
    ret = setData(status[0],2);//设置原账户注销

    ret = ret && insertRecord(0,rec);

    if(!(ret = submitAll()))
    {
        revert();//失败
        return ret;
    }

    QSqlQuery query(database());
    query.prepare("update Borrow set rdID = ? where rdID = ? ");
    query.addBindValue(rec.value(0));
    query.addBindValue(id.at(0));
    ret = query.exec();
    return ret;
}
