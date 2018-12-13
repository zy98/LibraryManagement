#ifndef BORROWMODEL_H
#define BORROWMODEL_H

#include "AbModel.h"



class BorrowInfoModel : public AbModel
{
public:
    BorrowInfoModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());

    bool returnBook(QItemSelectionModel* selection);
};

#endif // BORROWMODEL_H
