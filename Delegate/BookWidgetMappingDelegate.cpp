#include "BookWidgetMappingDelegate.h"

#include <QComboBox>
#include <QLabel>
#include <QDebug>

BookWidgetMappingDelegate::BookWidgetMappingDelegate(QObject* parent):QStyledItemDelegate(parent)
{

}

void BookWidgetMappingDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == 13)
    {
        auto cmb = static_cast<QComboBox*>(editor);
        if(cmb)
        {
            model->setData(index,cmb->currentIndex());
            return;
        }
    }
    QStyledItemDelegate::setModelData(editor,model,index);
}

void BookWidgetMappingDelegate::setEditorData
(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 13)
    {
        auto cmb = static_cast<QComboBox*>(editor);
        if(cmb)
        {
            cmb->setCurrentIndex(index.data().toInt());
            return;
        }
    }
    else if(index.column() == 12)
    {
        qDebug()<<"clear begin";
        auto data = index.data().toByteArray();
        auto label = static_cast<QLabel*>(editor);
        if(data.size() <= 0 || label == nullptr)
        {
            qDebug()<<"clear -1 return";
            label->clear();
            return;
        }
        qDebug()<<"clear 0 return";
        QPixmap pix;
        pix.loadFromData(index.data().toByteArray());
        pix = pix.scaledToWidth(label->width());
        label->setPixmap(pix);
        return;
    }
    QStyledItemDelegate::setEditorData(editor,index);
}
