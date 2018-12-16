#ifndef ABMODEL_H
#define ABMODEL_H

#include <QItemSelectionModel>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QString>



class AbModel : public QSqlRelationalTableModel
{
public:
    AbModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    virtual ~AbModel() = 0;

    static QSqlError seachReader(const QString& name, QSqlRecord& rec);
    static QSqlError borrowBookProc(const QString& rdid, long long bkid);

    QString U8(const char* s);

    virtual void setFetchTable(const QString& name);
    virtual void setManulSubmit();
    virtual void setAutoSubmit();

    virtual bool fetch();
    virtual bool submitData();

    virtual QString dbError();

    virtual bool insertItem(int count = 1);
    virtual bool fillRowItem(int row, QSqlRecord& rec);
    virtual bool deleteItem(QItemSelectionModel* selection);
    virtual bool selectItem(const QMap<QString,QVariant>& filter, bool flag);

    virtual bool uploadPicture(QItemSelectionModel* selection, QSharedPointer<QByteArray> dataImg);

protected:
    QSqlError error;
};

#endif // ABMODEL_H
