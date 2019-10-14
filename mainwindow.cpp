#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "video/videoMotion.h"

// para debug
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // openCV video habitaciones y configuracion
    int idVidCap = 0;
    for (int idDevice = 0; idDevice < MAX_ID_CAMS; idDevice++) {
        if( VideoMotion::isCamIdOk(idDevice) ){
            qDebug() << "Nueva camara: " << idVidCap +1 <<
                        " en id Devide: " << idDevice;
            videoApp[idVidCap] = new VideoMotion(idDevice,this);
            idVidCap ++;
        }
        if( idVidCap > CANT_HABITACIONES-1)
            break;
    }

    cams_activas = idVidCap;
    qDebug() << "Cantidad de camara: " << cams_activas;

    CamConnects();

}

MainWindow::~MainWindow()
{
    delete ui;
    for (int idCam = 0; idCam < cams_activas; idCam++) {
        videoApp[idCam]->mVideoCap.release();
    }
}


// establece los connects de las camaras
void MainWindow::CamConnects(){
    // para usar elementos de ui indexados (da error inexperados) lo hare sin for
    /*
    QLabel* opencvFrame[] = { ui->opencvFrame_1, ui->opencvFrame_2,
                             ui->opencvFrame_3, ui->opencvFrame_4 };
    QLabel* Habitacion[] = { ui->Label_Banio, ui->Label_Dormitorio,
                             ui->Label_Sala, ui->Label_Cocina };
    */
    // para usar path a imagenes
    QString PicsHabON[] = { ":/casa/banioOn.png", ":/casa/dormitorioOn.png",
                            ":/casa/livingOn.png", ":/casa/cocinaOn.png"};
    QString PicsHabOFF[] = { ":/casa/banioOff.png", ":/casa/dormitorioOff.png",
                             ":/casa/livingOff.png", ":/casa/cocinaOff.png" };

    //// Camara del banio
    int idCam = 0;
    if( cams_activas > idCam){
        // no anda idCam usado dentro de los connects, usa unica camara, raro
        connect(videoApp[0], &VideoMotion::newPixmapCapture,this,[&](){
            ui->opencvFrame_1->setPixmap( videoApp[0]->pixmap().scaled(320,240) );
            ui->camConfig_1->setPixmap( videoApp[0]->pixmapDST().scaled(320,240) );
        });
        // Presencias en habitacion
        connect(videoApp[0], &VideoMotion::presenciaDetectada,this,[&](){
            ui->Label_Banio->setPixmap(QPixmap( PicsHabON[0]));
        });
        connect(videoApp[0], &VideoMotion::sinPresencia,this,[&](){
            ui->Label_Banio->setPixmap(QPixmap( PicsHabOFF[0]));
        });
        videoApp[0]->start(QThread::HighPriority);
    }

    //// Camara del dormitorio
    idCam = 1;
    if( cams_activas > idCam){
        connect(videoApp[1], &VideoMotion::newPixmapCapture,this,[&](){
            ui->opencvFrame_2->setPixmap( videoApp[1]->pixmap().scaled(320,240) );
            ui->camConfig_2->setPixmap( videoApp[1]->pixmapDST().scaled(320,240) );
        });
        // Presencias en habitacion
        connect(videoApp[1], &VideoMotion::presenciaDetectada,this,[&](){
            ui->Label_Dormitorio->setPixmap(QPixmap( PicsHabON[1]));
        });
        connect(videoApp[1], &VideoMotion::sinPresencia,this,[&](){
            ui->Label_Dormitorio->setPixmap(QPixmap( PicsHabOFF[1]));
        });
        videoApp[1]->start(QThread::HighPriority);
    }

    //// Camara del living
    idCam = 2;
    if( cams_activas > idCam){
        connect(videoApp[2], &VideoMotion::newPixmapCapture,this,[&](){
            ui->opencvFrame_3->setPixmap(videoApp[2]->pixmap().scaled(320,240) );
            ui->camConfig_3->setPixmap(videoApp[2]->pixmapDST().scaled(320,240) );
        });
        // Presencias en habitacion
        connect(videoApp[2], &VideoMotion::presenciaDetectada,this,[&](){
            ui->Label_Sala->setPixmap(QPixmap( PicsHabON[2]));
        });
        connect(videoApp[2], &VideoMotion::sinPresencia,this,[&](){
            ui->Label_Sala->setPixmap(QPixmap( PicsHabOFF[2]));
        });
        videoApp[2]->start(QThread::HighPriority);
    }

    //// Camara del cocina
    idCam = 3;
    if( cams_activas > idCam){
        connect(videoApp[3], &VideoMotion::newPixmapCapture,this,[&](){
            ui->opencvFrame_4->setPixmap(videoApp[3]->pixmap().scaled(320,240) );
            ui->camConfig_4->setPixmap(videoApp[3]->pixmapDST().scaled(320,240) );
        });
        // Presencias en habitacion
        connect(videoApp[3], &VideoMotion::presenciaDetectada,this,[&](){
            ui->Label_Cocina->setPixmap(QPixmap( PicsHabON[3]));
        });
        connect(videoApp[3], &VideoMotion::sinPresencia,this,[&](){
            ui->Label_Cocina->setPixmap(QPixmap( PicsHabOFF[3]));
        });
        videoApp[3]->start(QThread::HighPriority);
    }

}
