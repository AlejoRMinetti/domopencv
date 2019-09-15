//#-------------------------------------------------
//#
//# Project created by QtCreator 2016-05-30T18:42:28
//# Author: Nauroze Hoath
//#
//#-------------------------------------------------

#ifndef CAMARACONFIG_H
#define CAMARACONFIG_H

#include <QMainWindow>
#include <QCloseEvent>
#include <sstream>
#include <string>
#include <iostream>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace Ui {
class camaraConfig;
}

class MainVideoCapture;

class camaraConfig : public QMainWindow
{
    Q_OBJECT

public:
    explicit camaraConfig(QWidget *parent = 0);
    ~camaraConfig();

private slots:
    void on_addButton_clicked();
    void on_defaultButton_clicked();
    ////////// update de slicers
    void UpdateDetectParametros();

private:
    Ui::camaraConfig *ui;
    MainVideoCapture *mOpenCV_videoCapture;
};

#endif // CAMARACONFIG_H
