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
    void first();
    void last();
    void next();
    void prev();

    void newItem(bool checked){}
    void changeItem(bool checked){}
    void deleteItem(){}
    void submitItem(){}
    void changePwd(){}

    void BorrowBook(long long book);

//    void createItem(QSqlRecord& rec){}

//    bool setRecord(const QSqlRecord& rec){return true;}

protected:
    void initView(){}
    void initModel(){}



private slots:
    void on_btn_find_clicked();

private:
    void setCheck(QString rdid = "", long long bkid = 0);

    Ui::BorrowWidget *ui;

    QString readerID;
};

#endif // BORROWWIDGET_H
