#ifndef BOOKMODEL_H
#define BOOKMODEL_H

#include "AbModel.h"

#include <QSqlTableModel>



class BookModel :public AbModel
{
public:
    BookModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());

    bool fillRowItem(int row, QSqlRecord &rec);

    bool uploadPicture(QItemSelectionModel* selection, QSharedPointer<QByteArray> dataImg);
};

#endif // BOOKMODEL_H
