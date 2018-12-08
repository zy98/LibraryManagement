#include "BorrowInfo.h"
#include "ui_BorrowInfo.h"

BorrowInfo::BorrowInfo(QWidget *parent) :
    InfoWidget(parent),
    ui(new Ui::BorrowInfo)
{
    ui->setupUi(this);
    //ui->groupBorrow->hide();
}

BorrowInfo::~BorrowInfo()
{
    delete ui;
}

void BorrowInfo::setWidgetMapper(QDataWidgetMapper* mapper)
{

}

void BorrowInfo::writeRecord(QSqlRecord& rec)
{

}
//设置Widget状态的函数;012->显示 新建 修改
void BorrowInfo::setWidgetStatus(int status)
{

}
void BorrowInfo::clear()
{

}

void BorrowInfo::setEnable(bool flag)
{

}
