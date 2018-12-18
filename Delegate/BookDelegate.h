#ifndef BOOKDELEGATE_H
#define BOOKDELEGATE_H

#include <QSqlRelationalDelegate>
#include <QStringListModel>



class BookDelegate : public QSqlRelationalDelegate
{
public:
    BookDelegate(QObject *parent = nullptr);

    //display
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    //QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    //edit
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    //void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    //void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QStringListModel* statusModel;
};

#endif // BOOKDELEGATE_H
