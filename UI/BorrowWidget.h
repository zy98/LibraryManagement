#ifndef BORROWWIDGET_H
#define BORROWWIDGET_H

#include "Widget.h"
#include "AbWidget.h"

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

    //直接返回指针不安全
    QTableView* viewPtr();
    AbModel* modelPtr();

public slots:
    void first();
    void last();
    void next();
    void prev();

    void newItem(bool checked){}
    void changeItem(bool checked){}

    void BorrowBook(long long book);
    void ReturnBook(QItemSelectionModel* selection);

protected:
    void initView(){}
    void initModel(){}



private slots:
    void on_btn_find_clicked();

private:
    Widget* currentWidget();

    Ui::BorrowWidget *ui;

    QSqlRecord readerRec;
};

#endif // BORROWWIDGET_H
