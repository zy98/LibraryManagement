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

    void setStatusFor(WidgetStatus status);

public slots:
    void newItem(bool checked){}
    void changeItem(bool checked){}
    void deleteItem(){}
    void submitItem(){}
    void changePwd(){}

//    void createItem(QSqlRecord& rec){}

//    bool setRecord(const QSqlRecord& rec){return true;}

protected:
    void initView(){}
    void initModel(){}

private:
    Ui::BorrowWidget *ui;


};

#endif // BORROWWIDGET_H
