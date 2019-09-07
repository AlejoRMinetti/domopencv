#ifndef REGISUSERS_H
#define REGISUSERS_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlTableModel>
#include "adduser.h"
#include "database.h"

namespace Ui {
class RegsUsers;
}

class DataBase;
class QSqlTableModel;

class RegsUsers : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegsUsers(DataBase * db, QWidget *parent = nullptr );
    ~RegsUsers();

private slots:
    void on_actionAtras_triggered();
    void on_actionAgregar_triggered();
    void on_actionSalir_triggered();

signals:

private:
    Ui::RegsUsers *ui;
    DataBase *datos;
    QSqlTableModel *model;
};

#endif // REGISUSERS_H
