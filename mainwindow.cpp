#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "video/mainvideocapture.h"

// para debug
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
/*
    // openCV video habitaciones y configuracion
    int idVidCap = 0;
    for (int idDevice = 0; idDevice < MAX_ID_CAMS; idDevice++) {
        if(MainVideoCapture::isCamIdOk(idDevice)){
            qDebug() << "Nueva camara: " << idVidCap +1 << " en id Devide: " << idDevice;
            videoCapture[idVidCap] = new MainVideoCapture(idDevice,this);
            idVidCap ++;
        }
        if( idVidCap > CANT_HABITACIONES-1)
            break;
    }
    cams_activas = idVidCap + 1;
    */

    // para probar una
    videoCapture[0] = new MainVideoCapture(0,this);
    cams_activas = 1;

    // para usar elementos de ui indexados
    QLabel* opencvFrame[] = { ui->opencvFrame_1, ui->opencvFrame_2,
                             ui->opencvFrame_3, ui->opencvFrame_4 };
    QLabel* Habitacion[] = { ui->Label_Banio, ui->Label_Dormitorio,
                             ui->Label_Sala, ui->Label_Cocina };
    QString PicsHabON[] = { ":/casa/banioOn.png", ":/casa/dormitorioOn.png",
                            ":/casa/livingOn.png", ":/casa/cocinaOn.png"};
    QString PicsHabOFF[] = { ":/casa/banioOff.png", ":/casa/dormitorioOff.png",
                             ":/casa/livingOff.png", ":/casa/cocinaOff.png" };

    // FIXME ver si el error esta en el metodo estatico, comparar con el objTracking que anda
    // para probar una
    connect(videoCapture[0], &MainVideoCapture::newPixmapCapture,this,[&](){
        opencvFrame[0]->setPixmap(
                    videoCapture[0]->getAllDetections().scaled(320,240));
                                          //opencvFrame[0]->width()-1,
                                          //opencvFrame[0]->height()-1)); // mostrar imagen RGB
        // usando .scaled(ui->opencvFrame->width(),ui->opencvFrame->height()) se va agrandando la pantalla sola!
    });
    // Presencias
    connect(videoCapture[0], &MainVideoCapture::presenciaDetectada,this,[&](){
        Habitacion[0]->setPixmap(QPixmap( PicsHabON[0]));
    });
    connect(videoCapture[0], &MainVideoCapture::sinPresencia,this,[&](){
        Habitacion[0]->setPixmap(QPixmap( PicsHabOFF[0]));
    });

    //conect para la camara a configurar
    connect(videoCapture[0], &MainVideoCapture::newPixmapCapture,this,[&](){
        ui->camRGB_all->setPixmap(videoCapture[0]->getAllDetections().scaled(320,240)); // mostrar imagen RGB
        ui->camRGB_actual->setPixmap(videoCapture[0]->getActualDetect().scaled(320,240)); // mostrar imagen // procesar y mostrar imagen HSV
        ui->camHSV->setPixmap(videoCapture[0]->getHSVpixmap().scaled(320,240));
        ui->cam_umbral->setPixmap(videoCapture[0]->getBinarioDetec().scaled(320,240));
        //UpdateDetectParametros(0);
    });

    // arraque desde el inicio las capturas
    videoCapture[0]->start(QThread::HighPriority);


    /*
    for (int idCam = 0; idCam < cams_activas; idCam++) {
        connect(videoCapture[idCam], &MainVideoCapture::newPixmapCapture,this,[&](){
            opencvFrame[idCam]->setPixmap(
                        videoCapture[idCam]->getAllDetections().scaled(
                                              opencvFrame[idCam]->width()-1,
                                              opencvFrame[idCam]->height()-1)); // mostrar imagen RGB
            // usando .scaled(ui->opencvFrame->width(),ui->opencvFrame->height()) se va agrandando la pantalla sola!
        });
        // Presencias
        connect(videoCapture[idCam], &MainVideoCapture::presenciaDetectada,this,[&](){
            Habitacion[idCam]->setPixmap(QPixmap( PicsHabON[idCam]));
        });
        connect(videoCapture[idCam], &MainVideoCapture::sinPresencia,this,[&](){
            Habitacion[idCam]->setPixmap(QPixmap( PicsHabOFF[idCam]));
        });
        // arraque desde el inicio las capturas
        videoCapture[idCam]->start(QThread::HighPriority);
    }
*/


}

MainWindow::~MainWindow()
{
    delete ui;
    for (int idCam = 0; idCam < cams_activas; idCam++) {
        videoCapture[idCam]->terminate();
    }
}

/////////////// Update parametros de configuracion de deteccion
void MainWindow::UpdateDetectParametros(int idCam){
    // get parametros para creacion de deteccion de bordes
    videoCapture[idCam]->H_MIN = ui->hminBox->text().toInt();
    videoCapture[idCam]->H_MAX = ui->hmaxBox->text().toInt();
    videoCapture[idCam]->S_MIN = ui->sminBox->text().toInt();
    videoCapture[idCam]->S_MAX = ui->smaxBox->text().toInt();
    videoCapture[idCam]->V_MIN = ui->vminBox->text().toInt();
    videoCapture[idCam]->V_MAX = ui->vmaxBox->text().toInt();
}

