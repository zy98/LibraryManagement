#ifndef BORROWINFOWIDGET_H
#define BORROWINFOWIDGET_H

#include "Widget.h"

#include <QSqlRelationalTableModel>

namespace Ui {
class BorrowInfoWidget;
}

class BorrowInfoWidget : public Widget
{
    Q_OBJECT

public:
    explicit BorrowInfoWidget(QWidget *parent = nullptr);
    ~BorrowInfoWidget();

    void setStatusFor(WidgetStatus status);

    void initView();
    void initModel();

    void setReader(const QString& rd);

public slots:
    void readRecord(const QSqlRecord &rec){ record = rec; }

private:
    Ui::BorrowInfoWidget *ui;
    QSqlRelationalTableModel* model;
};

#endif // BORROWINFOWIDGET_H
