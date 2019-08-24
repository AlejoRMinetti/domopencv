#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "video/mainvideocapture.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // openCV video
    mOpenCV_videoCapture = new MainVideoCapture(this);
    connect(mOpenCV_videoCapture, &MainVideoCapture::newPixmapCapture,this,[&](){
        ui->opencvFrame->setPixmap(mOpenCV_videoCapture->pixmap()/*.scaled(320,240)*/);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    mOpenCV_videoCapture->terminate();
}

void MainWindow::on_Tabs_tabBarClicked(int index)
{
    mOpenCV_videoCapture->start(QThread::HighPriority);
}
