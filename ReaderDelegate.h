#ifndef READERDELEGATE_H
#define READERDELEGATE_H

#include <QSqlRelationalDelegate>
#include <QStringListModel>



class ReaderDelegate : public QSqlRelationalDelegate
{
public:
    ReaderDelegate(QObject *parent = nullptr);
    //display
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    //edit
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QStringListModel* listModel;
    QStringListModel* typeModel;
};

#endif // READERDELEGATE_H
