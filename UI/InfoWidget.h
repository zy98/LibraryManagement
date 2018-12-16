#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include "AbWidget.h"

#include <QDataWidgetMapper>
#include <QSqlRecord>

class InfoWidget : public AbWidget
{
    Q_OBJECT

signals:
    void new_data(QSqlRecord&);
    void updatePicture(QSharedPointer<QByteArray> dataImg);

public:
    explicit InfoWidget(QWidget *parent = nullptr):AbWidget(parent){}
    ~InfoWidget(){}

    virtual void setWidgetMapper(QDataWidgetMapper* mapper) = 0;

    virtual void setStatusFor(WidgetStatus status) = 0;

public slots:    
    //如果record不为空，则通过record设置显示数据;record与所需数据不符的情形未定义
    virtual void readRecord(const QSqlRecord &rec){ record = rec; }

    virtual void writeRecord(QSqlRecord& rec) = 0;

    virtual void clear() = 0;

    virtual void setEnable(bool flag) = 0;

protected:
    QSqlRecord record;

};

#endif // INFOWIDGET_H
