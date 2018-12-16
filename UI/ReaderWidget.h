#ifndef READERWIDGET_H
#define READERWIDGET_H

#include <QSqlRelationalTableModel>
#include <Model/ReaderModel.h>
#include "Widget.h"
#include "ReaderInfo.h"

namespace Ui {
class ReaderWidget;
}

class ReaderWidget : public Widget
{
    Q_OBJECT

public:
    explicit ReaderWidget(QWidget *parent = nullptr);
    ~ReaderWidget();

    QTableView* viewPtr();
    AbModel* modelPtr();

public slots:
    void newItem(bool checked);
    void changeItem(bool checked);

    bool setRecord(const QSqlRecord& rec);
    bool createItem(QSqlRecord &rec);


private slots:
    void on_btn_find_clicked();

    void typeIndexChanged(int index);

    void on_btn_loss_clicked();
    
    void on_btn_user_disable_clicked();

    void on_btn_normal_clicked();

    void on_btn_reset_clicked();

    void on_btn_reapply_clicked();

private:
    void initView();
    void initModel();
    void setReaderStatus(int status);
    void setStatusFor(WidgetStatus status){}

    Ui::ReaderWidget *ui;
    ReaderModel* model;
};

#endif // READERWIDGET_H
