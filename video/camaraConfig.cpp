//#-------------------------------------------------
//#
//# Project created by QtCreator 2016-05-30T18:42:28
//# Author original: Nauroze Hoath
//#

//  Tutorial:
//  https://hetpro-store.com/TUTORIALES/category/programacion/opencv/
//  https://www.opencv-srf.com/2010/09/object-detection-using-color-seperation.html

//  Version adaptada para TPO Info2 por
//  Alejo R Minetti
//  David Ochoa
//#-------------------------------------------------

#include "video/camaraConfig.h"
#include "ui_camaraConfig.h"
#include "video/mainvideocapture.h"

using namespace cv;
using namespace std;

camaraConfig::camaraConfig(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::camaraConfig)
{
    ui->setupUi(this);

    // openCV video on GUI
    mOpenCV_videoCapture = new MainVideoCapture(this);
    connect(mOpenCV_videoCapture, &MainVideoCapture::newPixmapCapture,this,[&](){
        ui->opencvFrame->setPixmap(mOpenCV_videoCapture->getAllDetections().scaled(320,240)); // mostrar imagen RGB
        // usando .scaled(ui->opencvFrame->width(),ui->opencvFrame->height()) se va agrandando la pantalla sola!
        ui->opencvFrame_2->setPixmap(mOpenCV_videoCapture->getActualDetect().scaled(320,240)); // mostrar imagen // procesar y mostrar imagen HSV
        ui->opencvFrame_3->setPixmap(mOpenCV_videoCapture->getHSVpixmap().scaled(320,240));
        ui->opencvFrame_4->setPixmap(mOpenCV_videoCapture->getBinarioDetec().scaled(320,240));
        UpdateDetectParametros();
    });
    // arraque desde el inicio las capturas
    mOpenCV_videoCapture->start(QThread::HighPriority);
}

camaraConfig::~camaraConfig()
{
    delete ui;
    //mOpenCV_videoCapture->terminate();
    //mOpenCV_videoCapture->quit();
}

void camaraConfig::on_addButton_clicked()
{
    // todo agregar nueva deteccion
    mOpenCV_videoCapture->objects.push_back(mOpenCV_videoCapture->setUpObject(ui->nameLineEdit->text().toStdString(),
                                                        mOpenCV_videoCapture->H_MIN,
                                                        mOpenCV_videoCapture->H_MAX,
                                                        mOpenCV_videoCapture->S_MIN,
                                                        mOpenCV_videoCapture->S_MAX,
                                                        mOpenCV_videoCapture->V_MIN,
                                                        mOpenCV_videoCapture->V_MAX));
    ui->nameLineEdit->clear();
}

void camaraConfig::on_defaultButton_clicked()
{
    // todo guardar y reiniciar carga de deteccion
    ui->hminBox->setValue(0);
    ui->hmaxBox->setValue(255);
    ui->sminBox->setValue(0);
    ui->smaxBox->setValue(255);
    ui->vminBox->setValue(0);
    ui->vmaxBox->setValue(255);
}

////////// update de slicers
void camaraConfig::UpdateDetectParametros(){
    // get parametros para creacion de deteccion de bordes
    mOpenCV_videoCapture->H_MIN = ui->hminBox->text().toInt();
    mOpenCV_videoCapture->H_MAX = ui->hmaxBox->text().toInt();
    mOpenCV_videoCapture->S_MIN = ui->sminBox->text().toInt();
    mOpenCV_videoCapture->S_MAX = ui->smaxBox->text().toInt();
    mOpenCV_videoCapture->V_MIN = ui->vminBox->text().toInt();
    mOpenCV_videoCapture->V_MAX = ui->vmaxBox->text().toInt();
}
