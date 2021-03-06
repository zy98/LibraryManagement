﻿#ifndef BORROWINFOWIDGET_H
#define BORROWINFOWIDGET_H

#include "Widget.h"

#include <QSqlRelationalTableModel>

#include <Model/BorrowInfoModel.h>

namespace Ui {
class BorrowInfoWidget;
}

class BorrowInfoWidget : public Widget
{
    Q_OBJECT

signals:
    void returnBook(QItemSelectionModel* selection);

public:
    explicit BorrowInfoWidget(QWidget *parent = nullptr);
    ~BorrowInfoWidget();

    QTableView* viewPtr();
    AbModel* modelPtr();

    void setStatusFor(WidgetStatus status);

    void initView();
    void initModel();

    void setReader(const QString& rd);

public slots:
    void readRecord(const QSqlRecord &rec){ record = rec; }


private slots:
    void on_btn_returnBook_clicked();
    void BtnFilter(int id);

    void on_btn_search_clicked();

    void on_btn_reLend_clicked();

private:
    void newItem(){}
    void changeItem(){}

    Ui::BorrowInfoWidget *ui;
    BorrowInfoModel* model;
};

#endif // BORROWINFOWIDGET_H
