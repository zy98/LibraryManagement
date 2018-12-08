#ifndef BOOKWIDGET_H
#define BOOKWIDGET_H

#include "BookInfo.h"
#include "Widget.h"

#include <QSqlRecord>
#include <QSqlTableModel>

namespace Ui {
class BookWidget;
}

class BookWidget : public Widget
{
    Q_OBJECT

public:
    explicit BookWidget(QWidget *parent = nullptr);
    ~BookWidget();

public slots:
    void newItem(bool checked);
    void changeItem(bool checked);
    //void submitItem();
    void changePwd();

    void createItem(QSqlRecord &rec);

    bool setRecord(const QSqlRecord& rec);

protected:
    //void closeEvent(QCloseEvent* event);

protected slots:

private slots:
    void on_bkBtnFind_clicked();

private:
    void initView();
    void initModel();

    Ui::BookWidget *ui;
    QSqlTableModel* model;
    BookInfo* bookInfo;
};

#endif // BOOKWIDGET_H
