﻿#ifndef READERMODEL_H
#define READERMODEL_H

#include "AbModel.h"

#include <QItemSelectionModel>
#include <QSqlRelationalTableModel>



class ReaderModel : public AbModel
{
public:
    ReaderModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());

    static bool setLoss(const QString& name);

    bool setRdStatusLoss(QItemSelectionModel* selection);
    bool setRdStatusNomal(QItemSelectionModel* selection);
    bool setRdStatusDisabled(QItemSelectionModel* selection);

protected:
    bool setRdStatus(QItemSelectionModel* selection, int status);

    QString rdStatus;
};


#endif // READERMODEL_H
