#include "Database.h"

#include <QSqlQuery>
//#include <QVariant>

QSqlDatabase Database::database;

Database::Database(const QString& dbname)
{
    ConnectDB(dbname);
}

Database::~Database()
{
    if(database.isOpen())
        database.close();
}

bool Database::ConnectDB(const QString& dbname)
{
    if(!database.isOpen())
    {
        database = QSqlDatabase::addDatabase("QODBC",dbname);
        database.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                       "SERVER=%1;" //服务器名称
                                       "DATABASE=%2;"//数据库名
                                       "UID=%3;"           //登录名
                                       "PWD=%4;"        //密码
                                       ).arg("127.0.0.1")
                                       .arg(dbname)
                                       .arg("sa")
                                       .arg("19980206") );
        database.open();
    }
    return (database.isOpen() ? true : false);
}

bool Database::isOpen()
{
    return database.isOpen();
}
