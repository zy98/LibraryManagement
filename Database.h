#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QString>
#include <QFile>
#include <QSqlRecord>

class Database
{
public:
    Database(const QString &dbname);
    ~Database();

    //连接数据库并打开数据库，返回值表示是否成功打开数据库
    //dbname表示要连接的数据库名称，并且数据库连接名也会设置位dbname
    bool ConnectDB(const QString& dbname);

    //判断数据库是否打开
    bool isOpen();

private:
    static QSqlDatabase database;
};


#endif // CONNECTDATABASE_H
