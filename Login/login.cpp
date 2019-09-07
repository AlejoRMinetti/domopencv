#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

LogIn::LogIn(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogIn)
{
    ui->setupUi(this);
    //regs = new RegsUsers(this);
    mydb = new DataBase(this);
    if(!mydb->iniciaBaseDatos()){
        qDebug()<<mydb->getError();
    }
    if(!mydb->configuraBaseDatos()){
        qDebug()<<mydb->getError();
    }
}

LogIn::~LogIn()
{    
    delete ui;
}

void LogIn::on_actionSalir_triggered()
{
    mydb->closeData();
    close();
}

void LogIn::on_action_Config_triggered()
{
    regs = new RegsUsers(getMydb(), this);

    regs->show();
    this->setVisible(false);
}

void LogIn::on_pushButton_Acceder_clicked()
{
    QString username = ui->txtUser->text();
    QString password = ui->txtPass->text();

    if( !mydb->verifyData(username,password)){
        QMessageBox::warning(this,"Error","Usuario y/o contraseña inválida");
        return;
    }
    app = new Aplicacion(this);
    app->show();
    this->setVisible(false);
}

void LogIn::on_radioButton_toggled(bool checked)
{
    if(!checked){
        ui->txtPass->setEchoMode(QLineEdit::Password);
    } else {
        ui->txtPass->setEchoMode(QLineEdit::Normal);
    }
}

DataBase *LogIn::getMydb() const
{
    return mydb;
}
