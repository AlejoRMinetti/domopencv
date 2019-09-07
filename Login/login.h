#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QDebug>
#include "DataBase/regsUsers.h"
#include "mainwindow.h"

namespace Ui {
class LogIn;
}

class DataBase;

class LogIn : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = nullptr);
    ~LogIn();

    DataBase *getMydb() const;

private slots:
    void on_actionSalir_triggered();
    void on_action_Config_triggered();
    void on_pushButton_Acceder_clicked();

    void on_radioButton_toggled(bool checked);

private:
    Ui::LogIn *ui;
    RegsUsers *regs;
    MainWindow *app;
    DataBase *mydb;
};

#endif // LOGIN_H
