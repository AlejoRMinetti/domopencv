#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "video/mainvideocapture.h"
#include "video/camaraConfig.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // openCV video
    videoCapture = new MainVideoCapture(this);
    connect(videoCapture, &MainVideoCapture::newPixmapCapture,this,[&](){
        ui->opencvFrame->setPixmap(videoCapture->getAllDetections().scaled(320,240)); // mostrar imagen RGB
    });
    // arraque desde el inicio las capturas
    videoCapture->start(QThread::HighPriority);
}

MainWindow::~MainWindow()
{
    delete ui;
    videoCapture->terminate();
}

/////////// configuracion de camaras

void MainWindow::on_pushButton_config_cam1_released()
{
    camaraConfig w;
    w.show();
}
