﻿#include "Delegate/ReaderWidgetMappingDelegate.h"

#include <QComboBox>
#include <QLabel>
#include <QDebug>

ReaderWidgetMappingDelegate::ReaderWidgetMappingDelegate(QObject* parent):
    QStyledItemDelegate(parent)
{

}

void ReaderWidgetMappingDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == 4)
    {
        auto cmb = static_cast<QComboBox*>(editor);
        if(cmb)
            model->setData(index,cmb->currentIndex() + 1);
        return;
    }
    else if(index.column() == 10)
    {
        auto cmb = static_cast<QComboBox*>(editor);
        if(cmb)
            model->setData(index,cmb->currentIndex());
        return;
    }
    QStyledItemDelegate::setModelData(editor,model,index);
}

void ReaderWidgetMappingDelegate::setEditorData
(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 10)
    {
        auto cmb = static_cast<QComboBox*>(editor);
        if(cmb)
            cmb->setCurrentIndex(index.data().toInt());
        return;
    }
    else if(index.column() == 9)
    {
        auto data = index.data().toByteArray();
        auto label = static_cast<QLabel*>(editor);
        if(data.size() <= 0 || label == nullptr)
        {
            label->clear();
            return;
        }
        QPixmap pix;
        pix.loadFromData(index.data().toByteArray());
        label->setPixmap(pix);
        return;
    }
    QStyledItemDelegate::setEditorData(editor,index);
}
