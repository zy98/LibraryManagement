#include "BookDelegate.h"

#include <QDateEdit>
#include <QPainter>

BookDelegate::BookDelegate(QObject* parent):QSqlRelationalDelegate(parent)
{
    QStringList list;
    list<<QString::fromLocal8Bit("在馆")
          <<QString::fromLocal8Bit("借出")
            <<QString::fromLocal8Bit("销毁");
    statusModel = new QStringListModel(list);
}

//display
void BookDelegate::paint
(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 13)
    {
        QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled) ?
            (option.state & QStyle::State_Active) ?
                        QPalette::Normal :
                        QPalette::Inactive :
                        QPalette::Disabled;

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect,option.palette.color(cg, QPalette::Highlight));

        int data = index.data().toInt();
        painter->drawText(option.rect,Qt::AlignVCenter,statusModel->index(data).data().toString());
        return;
    }
    return QSqlRelationalDelegate::paint(painter,option,index);
}

//edit
QWidget* BookDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 13)//bkStatus
    {
        QComboBox* box = new QComboBox(parent);
        if(box)
        {
            box->setModel(statusModel);
            box->setCurrentIndex(index.data().toInt());
            return box;
        }
    }
    else if(index.column() == 4 || index.column() == 10)
    {
        QDateEdit* date = new QDateEdit(parent);
        if(date)
        {
            date->setDate(index.data().toDate());
            return date;
        }
    }
    return QSqlRelationalDelegate::createEditor(parent,option,index);
}
void BookDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == 13)
    {
        auto box = static_cast<QComboBox*>(editor);
        if(box)
            model->setData(index,box->currentIndex());

        return;
    }
    else if(index.column() == 4 || index.column() == 10)
    {
        auto edit = static_cast<QDateEdit*>(editor);
        if(edit)
            model->setData(index,edit->date().toString("yyyy-MM-dd"));

        return;
    }
    QSqlRelationalDelegate::setModelData(editor,model,index);
}
