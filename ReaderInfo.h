#ifndef READERINFO_H
#define READERINFO_H

#include <QAbstractTableModel>
#include <QDataWidgetMapper>
#include <QSqlRecord>
#include "InfoWidget.h"

namespace Ui {
class ReaderInfo;
}

class ReaderInfo : public InfoWidget
{
    Q_OBJECT

public:
    explicit ReaderInfo(QWidget *parent = nullptr);
    ~ReaderInfo();

    //设置控件映射
    void setWidgetMapper(QDataWidgetMapper* mapper);

    void setStatusFor(WidgetStatus status);

    //初始化读者类型控件
    //void setTypeModel(QAbstractItemModel *model, int col);

    //清空所用输入栏，并将借书数量栏默认0，读者类别默认 本科生，办证日期默认为当天系统日期
    void clear();



public slots:
    //如果record不为空，则通过record设置显示数据;record与所需数据不符的情形未定义
    void readRecord(const QSqlRecord &rec);

    void writeRecord(QSqlRecord& rec);

private slots:
    //挂失按钮，0表示正常，1表示挂失，根据状态设置证件状态和挂失按钮
    void on_btn_loss_clicked();

    void on_btn_pwd_clicked();

    void on_btn_upload_clicked();

    void on_btn_submit_clicked();

private:
    //设置禁用/启用lineedit控件
    void setEnable(bool flag);

private:
    Ui::ReaderInfo *ui;
    int flag;
};

#endif // READERINFO_H
