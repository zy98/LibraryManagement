#ifndef BORROWWIDGET_H
#define BORROWWIDGET_H

#include "Widget.h"

namespace Ui {
class BorrowWidget;
}

class BorrowWidget : public Widget
{
    Q_OBJECT

public:
    explicit BorrowWidget(QWidget *parent = nullptr);
    ~BorrowWidget();

public slots:
    virtual void newItem(bool checked){}
    virtual void changeItem(bool checked){}
    virtual void deleteItem(){}
    virtual void submitItem(){}
    virtual void changePwd(){}

    virtual void createItem(const QSqlRecord& rec){}

    virtual bool setRecord(const QSqlRecord& rec){return true;}

protected:
    virtual void initView(){}
    virtual void initModel(){}

private:
    Ui::BorrowWidget *ui;


};

#endif // BORROWWIDGET_H
