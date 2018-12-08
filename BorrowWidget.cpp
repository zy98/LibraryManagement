#include "BorrowWidget.h"
#include "ui_BorrowWidget.h"

BorrowWidget::BorrowWidget(QWidget *parent) :
    Widget(parent),
    ui(new Ui::BorrowWidget)
{
    ui->setupUi(this);
    ui->borrow->setStatusFor(Reader);
    ui->reader->setStatusFor(Display);
}

BorrowWidget::~BorrowWidget()
{
    delete ui;
}

//for BorrowAdmin Reader
void BorrowWidget::setStatusFor(WidgetStatus status)
{
    if(status == BorrowAdmin)
    {
        ui->borrow->setStatusFor(BorrowAdmin);
        ui->book->setStatusFor(BorrowAdmin);
        return;
    }

    ui->borrow->setStatusFor(Reader);
    ui->book->setStatusFor(Reader);
}

