#include "BookModel.h"

BookModel::BookModel(QObject *parent, QSqlDatabase db):
    AbModel(parent,db)
{
    setFetchTable("Book");

    setHeaderData(0,Qt::Horizontal,U8("图书编号"));
    setHeaderData(1,Qt::Horizontal,U8("书名"));
    setHeaderData(2,Qt::Horizontal,U8("作者"));
    setHeaderData(3,Qt::Horizontal,U8("出版社"));
    setHeaderData(4,Qt::Horizontal,U8("出版日期"));
    setHeaderData(5,Qt::Horizontal,U8("ISBN"));
    setHeaderData(6,Qt::Horizontal,U8("分类"));
    setHeaderData(7,Qt::Horizontal,U8("语言"));
    setHeaderData(8,Qt::Horizontal,U8("页数"));
    setHeaderData(9,Qt::Horizontal,U8("价格"));
    setHeaderData(10,Qt::Horizontal,U8("入库日期"));
    setHeaderData(11,Qt::Horizontal,U8("简介"));
    setHeaderData(12,Qt::Horizontal,U8("封面"));
    setHeaderData(13,Qt::Horizontal,U8("状态"));
    setHeaderData(14,Qt::Horizontal,U8("数量"));
}

bool BookModel::fillRowItem(int row, QSqlRecord &rec)
{
    bool ret = true;
    for(int i = 1; i < rec.count(); i++)
        ret = setData(index(row,i), rec.value(i));
    return ret;
}
