#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#define   MAX_USER  10

class Usuario
{
public:
    Usuario(const QString &username = "",const QString &password = "");
    QString userName()const {   return username;  }
    QString Password()const {   return password;  }
private:
    QString username;
    QString password;
};

class DataBase : public QObject
{
   Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr );
    bool iniciaBaseDatos();
    bool configuraBaseDatos();
    QString getError() const { return mError; }
    bool insertaUsuario(const Usuario &user);
    bool verifyData( const QString username,const QString password );
    void closeData();
    QSqlDatabase getDb() const;

private:
    QSqlDatabase db;
    // TODO tableView con SQL
    QString mError;
};

#endif // DATABASE_H
