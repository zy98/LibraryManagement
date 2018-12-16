#ifndef WIDGET_H
#define WIDGET_H

#include "AbWidget.h"

#include <QWidget>
#include <QMessageBox>
#include <QTableView>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>

#include <Model/AbModel.h>

class Widget : public AbWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void setStatusFor(WidgetStatus status) = 0;

    virtual AbModel* modelPtr() = 0;
    virtual QTableView* viewPtr() = 0;

public slots:
    void first();
    void last();
    void next();
    void prev();
    virtual void newItem(bool checked) = 0;
    virtual void changeItem(bool checked) = 0;
    virtual void deleteItem();
    virtual void submitItem();
    virtual void changePwd();
    virtual bool createItem(QSqlRecord& rec);

    virtual bool setRecord(const QSqlRecord& rec);
    virtual bool updatePicture(QSharedPointer<QByteArray> dataImg);

protected:
    virtual void initView(){}
    virtual void initModel(){}

    QSqlRecord record;
};

#endif // WIDGET_H
