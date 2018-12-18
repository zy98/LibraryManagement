#ifndef BOOKWIDGETMAPPINGDELEGATE_H
#define BOOKWIDGETMAPPINGDELEGATE_H

#include <QStyledItemDelegate>



class BookWidgetMappingDelegate : public QStyledItemDelegate
{
public:
    BookWidgetMappingDelegate(QObject* parent = nullptr);

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
};

#endif // BOOKWIDGETMAPPINGDELEGATE_H
