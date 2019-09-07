#include "regsUsers.h"
#include "ui_regsUsers.h"
#include "Login/login.h"
#include "QDebug"

RegsUsers::RegsUsers(DataBase *db, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegsUsers)
{
    ui->setupUi(this);
    // base de datos y modelo para mostrar en tabla
    datos = db;
    model = new QSqlTableModel(this, datos->getDb());

    // FIXME intento de hacer que muestre sql mediante el ejemplo Cached Table Example

    model->setTable("Usuarios registrados");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    //model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Nombre"));
    model->setHeaderData(2, Qt::Horizontal, tr("Password"));

    ui->tableView_Users->setModel(model);
    ui->tableView_Users->resizeColumnsToContents();

}

RegsUsers::~RegsUsers()
{
    delete ui;
}

void RegsUsers::on_actionAtras_triggered()
{
    LogIn *app = new LogIn;
    app->show();
    this->setVisible(false);

}

void RegsUsers::on_actionAgregar_triggered()
{
    int res;
    AddUser *addUser = new AddUser(this);
    res = addUser->exec();

    // respuestas del dialogo AddUser
    if(res == QDialog::Rejected){
        return;
    }
    if(!datos->insertaUsuario(addUser->usuario())){
        QMessageBox::critical(this,"Error",datos->getError());
        qDebug()<<datos->getError();
        return;
    }
    if(model){
        model->select();
    }
}


void RegsUsers::on_actionSalir_triggered()
{
    close();
}
