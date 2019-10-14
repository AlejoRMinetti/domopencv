#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "video/videoMotion.h"

#define CANT_HABITACIONES 4
#define MAX_ID_CAMS 10

namespace Ui {
class MainWindow;
}

class VideoMotion;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    VideoMotion *videoApp[CANT_HABITACIONES];
    int cams_activas = 0;

    // establece los connects de las camaras
    void CamConnects();
    // Update parametros de configuracion de deteccion
    void UpdateDetectParametros(int idCam);

};

#endif // MAINWINDOW_H
