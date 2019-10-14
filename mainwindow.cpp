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

    // openCV video habitaciones y configuracion
    int idVidCap = 0;
    for (int idDevice = 0; idDevice < MAX_ID_CAMS; idDevice++) {
        if( MainVideoCapture::isCamIdOk(idDevice) ){
            qDebug() << "Nueva camara: " << idVidCap +1 << " en id Devide: " << idDevice;
            videoCapture[idVidCap] = new MainVideoCapture(idDevice,this);
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
        connect(videoCapture[idCam], &MainVideoCapture::newPixmapCapture,this,[&](){
            ui->opencvFrame_1->setPixmap(
                        videoCapture[idCam]->getAllDetections().scaled(320,240));
            // usando .scaled(ui->opencvFrame->width(),ui->opencvFrame->height()) se va agrandando la pantalla sola!
        });
        // Presencias en habitacion
        connect(videoCapture[idCam], &MainVideoCapture::presenciaDetectada,this,[&](){
            ui->Label_Banio->setPixmap(QPixmap( PicsHabON[idCam]));
        });
        connect(videoCapture[idCam], &MainVideoCapture::sinPresencia,this,[&](){
            ui->Label_Banio->setPixmap(QPixmap( PicsHabOFF[idCam]));
        });
        videoCapture[idCam]->start(QThread::HighPriority);
    }

    //// Camara del dormitorio
    idCam = 1;
    if( cams_activas > idCam){
        connect(videoCapture[idCam], &MainVideoCapture::newPixmapCapture,this,[&](){
            ui->opencvFrame_2->setPixmap(
                        videoCapture[idCam]->getAllDetections().scaled(320,240));
        });
        // Presencias en habitacion
        connect(videoCapture[idCam], &MainVideoCapture::presenciaDetectada,this,[&](){
            ui->Label_Dormitorio->setPixmap(QPixmap( PicsHabON[idCam]));
        });
        connect(videoCapture[idCam], &MainVideoCapture::sinPresencia,this,[&](){
            ui->Label_Dormitorio->setPixmap(QPixmap( PicsHabOFF[idCam]));
        });
        videoCapture[idCam]->start(QThread::HighPriority);
    }

    //// Camara del living
    idCam = 2;
    if( cams_activas > idCam){
        connect(videoCapture[idCam], &MainVideoCapture::newPixmapCapture,this,[&](){
            ui->opencvFrame_3->setPixmap(
                        videoCapture[idCam]->getAllDetections().scaled(320,240));
        });
        // Presencias en habitacion
        connect(videoCapture[idCam], &MainVideoCapture::presenciaDetectada,this,[&](){
            ui->Label_Sala->setPixmap(QPixmap( PicsHabON[idCam]));
        });
        connect(videoCapture[idCam], &MainVideoCapture::sinPresencia,this,[&](){
            ui->Label_Sala->setPixmap(QPixmap( PicsHabOFF[idCam]));
        });
        videoCapture[idCam]->start(QThread::HighPriority);
    }

    //// Camara del cocina
    idCam = 3;
    if( cams_activas > idCam){
        connect(videoCapture[idCam], &MainVideoCapture::newPixmapCapture,this,[&](){
            ui->opencvFrame_4->setPixmap(
                        videoCapture[idCam]->getAllDetections().scaled(320,240));
        });
        // Presencias en habitacion
        connect(videoCapture[idCam], &MainVideoCapture::presenciaDetectada,this,[&](){
            ui->Label_Cocina->setPixmap(QPixmap( PicsHabON[idCam]));
        });
        connect(videoCapture[idCam], &MainVideoCapture::sinPresencia,this,[&](){
            ui->Label_Cocina->setPixmap(QPixmap( PicsHabOFF[idCam]));
        });
        videoCapture[idCam]->start(QThread::HighPriority);
    }


    //conect para la camara a configurar
    connect(videoCapture[0], &MainVideoCapture::newPixmapCapture,this,[&](){
        ui->camRGB_all->setPixmap(videoCapture[0]->getAllDetections().scaled(320,240)); // mostrar imagen RGB
        ui->camRGB_actual->setPixmap(videoCapture[0]->getActualDetect().scaled(320,240)); // mostrar imagen // procesar y mostrar imagen HSV
        ui->camHSV->setPixmap(videoCapture[0]->getHSVpixmap().scaled(320,240));
        ui->cam_umbral->setPixmap(videoCapture[0]->getBinarioDetec().scaled(320,240));
        UpdateDetectParametros(0);
    });

}
