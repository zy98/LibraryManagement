#ifndef WIDGET_H
#define WIDGET_H

#include "AbWidget.h"

#include <QWidget>
#include <QMessageBox>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlRecord>

class Widget : public AbWidget
{
    Q_OBJECT

signals:
    void status_changed(int status);

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void first();
    void last();
    void next();
    void prev();
    virtual void newItem(bool checked);
    virtual void changeItem(bool checked);
    virtual void deleteItem();
    virtual void submitItem();
    virtual void changePwd();

    virtual void createItem(QSqlRecord& rec);

    virtual bool setRecord(const QSqlRecord& rec);

protected:
    virtual void initView(){}
    virtual void initModel(){}

    virtual void submitData();
    virtual QString lastError();

    QTableView* view;
    QSqlTableModel* tabModel;
    QSqlRecord record;
};

#endif // WIDGET_H
