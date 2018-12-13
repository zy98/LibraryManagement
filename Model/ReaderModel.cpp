#include "ReaderModel.h"

#include <QModelIndex>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


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

bool ReaderModel::setLoss(const QString& name)
{
    QSqlDatabase db = QSqlDatabase::database("Library");
    QSqlQuery query(db);
    query.prepare("update Reader set rdStatus = ? where rdID = ?");

    query.addBindValue(1);
    query.addBindValue(name);

    return query.exec();
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

    return setRdStatus(selection, 2);
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

