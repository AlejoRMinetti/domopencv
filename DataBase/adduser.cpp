#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QCompleter>
#include <QFileSystemModel>
#include "adduser.h"
#include "ui_adduser.h"
#include "database.h"
#define KEY     "AB-CDEF-GH"

AddUser::AddUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);
}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::on_buttonBox_rejected()
{
    reject();
}

void AddUser::on_buttonBox_accepted()
{
    user = Usuario(ui->lineEdit_UserName->text(),ui->lineEdit_Password->text());
    accept();
}
