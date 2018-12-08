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

public slots:
    void readRecord(const QSqlRecord &rec){ record = rec; }

    void writeRecord(QSqlRecord& rec);

    //设置Widget状态的函数;012->显示 新建 修改
    void setWidgetStatus(int status);
    void clear();

    void setEnable(bool flag);

private:
    Ui::BorrowInfo *ui;
};

#endif // BORROWINFO_H
