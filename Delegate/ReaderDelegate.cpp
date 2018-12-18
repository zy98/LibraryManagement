#include "ReaderDelegate.h"

#include <QLineEdit>
#include <QPainter>
#include <QSpinBox>
#include <QDebug>
#include <QDateEdit>
#include <QPalette>


ReaderDelegate::ReaderDelegate(QObject *parent)
    :QSqlRelationalDelegate(parent)
{
    QStringList list;
    list<<QString::fromLocal8Bit("男")<<QString::fromLocal8Bit("女");
    listModel = new QStringListModel(list,this);
    list.clear();
    list<<QString::fromLocal8Bit("正常")
       <<QString::fromLocal8Bit("挂失")
      <<QString::fromLocal8Bit("注销");
    typeModel = new QStringListModel(list,this);

}

//display
void ReaderDelegate::paint
(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 1)
    {
        QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled) ?
            (option.state & QStyle::State_Active) ?
                        QPalette::Normal :
                        QPalette::Inactive :
                        QPalette::Disabled;

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect,option.palette.color(cg, QPalette::Highlight));

        auto rect = option.rect;
        painter->setBrush(QBrush(Qt::lightGray));
        painter->drawRect(rect);
        return;
    }

    if(index.column()  == 10)
    {
        QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled) ?
            (option.state & QStyle::State_Active) ?
                        QPalette::Normal :
                        QPalette::Inactive :
                        QPalette::Disabled;

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect,option.palette.color(cg, QPalette::Highlight));

        auto data = index.data().toInt();
        painter->drawText(option.rect,Qt::AlignCenter,typeModel->index(data).data().toString());

        return;
    }

    QSqlRelationalDelegate::paint(painter,option,index);
}

QSize ReaderDelegate::sizeHint
(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSqlRelationalDelegate::sizeHint(option,index);// + QSize(2,2);
}

//edit
QWidget* ReaderDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 3)
    {
        auto combo = new QComboBox(parent);
        combo->setModel(listModel);
        return combo;
    }
    else if(index.column() == 8)
    {
        auto date =new QDateEdit(parent);
        return date;
    }
    else if(index.column() == 10)
    {
        auto box = new QComboBox(parent);
        if(box)
        {
            box->setModel(typeModel);
            return box;
        }
    }
    else if(index.column() == 0 || index.column() == 6)
        return nullptr;

    return QSqlRelationalDelegate::createEditor(parent, option, index);
}

void ReaderDelegate::setEditorData
(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 10)
    {
        auto box = dynamic_cast<QComboBox*>(editor);
        if(box) box->setCurrentIndex(index.data().toInt());
        return;
    }

    if(index.column() == 8)
    {
        auto date = dynamic_cast<QDateEdit*>(editor);
        if(date) date->setDate(index.data().toDate());
        return;
    }

    if(index.column() == 1)
        return;

    QSqlRelationalDelegate::setEditorData(editor,index);
}

void ReaderDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == 1)
    {
        auto edit = dynamic_cast<QLineEdit*>(editor);
        if(edit)
        {
            auto text = edit->text();
            if(!text.isEmpty())
                model->setData(index,text);
        }
        return;
    }

//    if(index.column() == 4)
//    {
//        auto box =static_cast<QComboBox*>(editor);
//        if(box)
//        {
//            qDebug()<<"box";
//            model->setData(index,box->currentIndex() + 1);
//            qDebug()<<index.data().toInt();
//        }
//        return;
//    }

    if(index.column() == 10)
    {
        auto box = dynamic_cast<QComboBox*>(editor);
        if(box) model->setData(index,box->currentIndex());
        return;
    }

    QSqlRelationalDelegate::setModelData(editor,model,index);
}

void ReaderDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSqlRelationalDelegate::updateEditorGeometry(editor,option,index);
}
