#include "BookWidgetMappingDelegate.h"

#include <QComboBox>
#include <QLabel>

BookWidgetMappingDelegate::BookWidgetMappingDelegate(QObject* parent):QObject(parent)
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
    QStyledItemDelegate::setModelData(editor,model,index);
}
