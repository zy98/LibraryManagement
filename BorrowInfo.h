#ifndef BORROWINFO_H
#define BORROWINFO_H

#include "InfoWidget.h"

namespace Ui {
class BorrowInfo;
}

class BorrowInfo : public InfoWidget
{
    Q_OBJECT

public:
    explicit BorrowInfo(QWidget *parent = nullptr);
    ~BorrowInfo();

    void setWidgetMapper(QDataWidgetMapper* mapper);

    void setStatusFor(WidgetStatus status);

public slots:
    void readRecord(const QSqlRecord &rec){ record = rec; }

    void writeRecord(QSqlRecord& rec);

    void clear();

    void setEnable(bool flag);

private:
    Ui::BorrowInfo *ui;
};

#endif // BORROWINFO_H
