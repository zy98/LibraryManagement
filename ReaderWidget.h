#ifndef READERWIDGET_H
#define READERWIDGET_H

#include <QSqlRelationalTableModel>
#include "Widget.h"
#include "ReaderInfo.h"

namespace Ui {
class ReaderWidget;
}

class ReaderWidget : public Widget
{
    Q_OBJECT

signals:


public:
    explicit ReaderWidget(QWidget *parent = nullptr);
    ~ReaderWidget();

public slots:
    void newItem(bool checked);
    void changeItem(bool checked);
    //void deleteItem();
    //void submitItem();
    void changePwd();

    bool setRecord(const QSqlRecord& rec);

    //void createItem(QSqlRecord &rec);

protected:
    void closeEvent(QCloseEvent* event);

    //void submitData();
    //inline QString lastError();

private slots:
    void on_btn_find_clicked();

    void typeIndexChanged(int index);

    void on_btn_loss_clicked();
    
    void on_btn_user_disable_clicked();

private:
    void initView();
    void initModel();

    Ui::ReaderWidget *ui;
    QSqlRelationalTableModel* model;
};

#endif // READERWIDGET_H
