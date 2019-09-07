#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDialog>
#include "database.h"

namespace Ui {
class AddUser;
}

class AddUser : public QDialog
{
    Q_OBJECT

public:
    explicit AddUser(QWidget *parent = nullptr);
    ~AddUser();
    Usuario usuario() const {   return user;    }
private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::AddUser *ui;
    Usuario user;
};

#endif // ADDUSER_H
