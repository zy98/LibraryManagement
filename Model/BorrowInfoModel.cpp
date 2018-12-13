#include "BorrowInfoModel.h"

#include <QDate>
#include <QSqlQuery>
#include <QDebug>

BorrowInfoModel::BorrowInfoModel(QObject *parent, QSqlDatabase db):
    AbModel(parent,db)
{
    setFetchTable("Borrow");

    setHeaderData(0,Qt::Horizontal,U8("借阅编号"));
    setHeaderData(1,Qt::Horizontal,U8("图书编号"));
    setHeaderData(2,Qt::Horizontal,U8("读者编号"));
    setHeaderData(3,Qt::Horizontal,U8("借书管理员"));
    setHeaderData(4,Qt::Horizontal,U8("还书管理员"));
    setHeaderData(5,Qt::Horizontal,U8("借书日期"));
    setHeaderData(6,Qt::Horizontal,U8("还书日期"));
    setHeaderData(7,Qt::Horizontal,U8("实际日期"));
    setHeaderData(8,Qt::Horizontal,U8("续借次数"));
    setHeaderData(9,Qt::Horizontal,U8("逾期天数"));
    setHeaderData(10,Qt::Horizontal,U8("罚款"));
    setHeaderData(11,Qt::Horizontal,U8("其他"));
}

bool BorrowInfoModel::returnBook(QItemSelectionModel* selection)
{
    bool ret = true;
    if(selection != nullptr)
    {
        QSqlDatabase db = QSqlDatabase::database("Library");
        auto rowList = selection->selectedIndexes();
        QSqlQuery query(db);
        query.prepare("exec usp_return_book @BorrowID = ? ");
        auto data = rowList.at(0).data().toLongLong();
        qDebug()<<data;
        query.addBindValue(data);

        ret = query.exec();
        qDebug()<<"ret:"<<ret;
    }
    return ret;
}
