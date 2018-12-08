#include "BorrowWidget.h"
#include "ui_BorrowWidget.h"

BorrowWidget::BorrowWidget(QWidget *parent) :
    Widget(parent),
    ui(new Ui::BorrowWidget)
{
    ui->setupUi(this);
    //ui->group_find->hide();
}

BorrowWidget::~BorrowWidget()
{
    delete ui;
}

