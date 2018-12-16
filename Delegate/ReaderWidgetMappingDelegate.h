#ifndef READERWIDGETMAPPINGDELEGATE_H
#define READERWIDGETMAPPINGDELEGATE_H

#include <QStyledItemDelegate>


class ReaderWidgetMappingDelegate : public QStyledItemDelegate
{
public:
    ReaderWidgetMappingDelegate(QObject* parent = nullptr);

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
};

#endif // READERWIDGETMAPPINGDELEGATE_H
