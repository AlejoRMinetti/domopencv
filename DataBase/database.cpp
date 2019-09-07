#include "database.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#define CREATE_TABLE   "CREATE TABLE IF NOT EXISTS USERS" \
                       "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL" \
                       ", name VARCHAR(30) NOT NULL" \
                       ", password VARCHAR(30) NOT NULL);"

#define FILE_NAME   "database.db"

////////////// Clase Usuario

Usuario::Usuario(const QString &username,const QString &password)
{
    this->password = password;
    this->username = username;
}

/////////// Clase DataBase

DataBase::DataBase(QObject *parent)
    : QObject(parent)
{
}

bool DataBase::iniciaBaseDatos()
{
    db = QSqlDatabase::addDatabase("QSQLITE"); /*Seleccionamos el modulo QSQLITE*/
    db.setDatabaseName(FILE_NAME);             /*Seleccionamos la base de datos creada*/
    bool ok = db.open();                       /*Abrimos la base de datos*/
    if (!ok) {
        mError = db.lastError().text();
    }
    return ok;
}

bool DataBase::configuraBaseDatos()
{
    QSqlQuery qry;
    bool ok = qry.exec(CREATE_TABLE);
    if (!ok) {
        mError = qry.lastError().text();
    }
    return ok;
}

bool DataBase::insertaUsuario(const Usuario &user)
{
    QSqlQuery q;
    q.prepare("INSERT INTO USERS (name, password ) VALUES (?,?)");
    q.addBindValue(user.userName());
    q.addBindValue(user.Password());
    bool ok = q.exec();
    if (!ok) {
        mError = q.lastError().text();
    }
    return ok;
}

bool DataBase::verifyData(const QString username, const QString password)
{
    QSqlQuery qry;
    qry.exec("SELECT * FROM USERS;");
    while (qry.next()) {
        QString name = QString::fromUtf8(qry.value(1).toString().toStdString().c_str());
        QString pass = QString::fromUtf8(qry.value(2).toString().toStdString().c_str());
        if(name == username && pass == password ){
            return true;
        }
    }
    return false;
}



void DataBase::closeData()
{
    db.close();
}

QSqlDatabase DataBase::getDb() const
{
    return db;
}


