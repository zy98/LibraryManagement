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

//for BorrowAdmin Reader
void BorrowInfo::setStatusFor(WidgetStatus status)
{
    if(status == BorrowAdmin)
        ui->groupBorrow->show();
    else
        ui->groupBorrow->hide();
}

void BorrowInfo::writeRecord(QSqlRecord& rec)
{

}

void BorrowInfo::clear()
{

}

void BorrowInfo::setEnable(bool flag)
{

}
