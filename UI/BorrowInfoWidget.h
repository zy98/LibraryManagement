#ifndef BORROWINFOWIDGET_H
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

public:
    explicit BorrowInfoWidget(QWidget *parent = nullptr);
    ~BorrowInfoWidget();

    QTableView* viewPtr();
    AbModel* modelPtr();

    void setStatusFor(WidgetStatus status);

    void newItem(bool checked){}
    void changeItem(bool checked){}

    void initView();
    void initModel();

    void setReader(const QString& rd);

public slots:
    void readRecord(const QSqlRecord &rec){ record = rec; }

private slots:
    void on_btn_returnBook_clicked();

private:
    Ui::BorrowInfoWidget *ui;
    BorrowInfoModel* model;
};

#endif // BORROWINFOWIDGET_H
