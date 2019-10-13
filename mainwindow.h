#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "video/mainvideocapture.h"

#define CANT_HABITACIONES 4

namespace Ui {
class MainWindow;
}

class MainVideoCapture;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MainVideoCapture *videoCapture[CANT_HABITACIONES];
    int cams_activas = 0;

    // Update parametros de configuracion de deteccion
    void UpdateDetectParametros(int idCam);
};

#endif // MAINWINDOW_H
