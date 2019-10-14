#ifndef VIDEOMOTION_H
#define VIDEOMOTION_H

#include <QPixmap>
#include <QImage>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <iostream>

using namespace cv;
using namespace std;

class VideoMotion : public QThread
{
    Q_OBJECT
public:
    VideoMotion(int idDevice, QObject *parent = nullptr, double min_area_incial = 50);
    QPixmap pixmap() const { return mPixmap; }
    QPixmap pixmapDif()const{ return mPixmapDif; }
    QPixmap pixmapDST()const{ return mPixmapDST; }
    // generador de imagenes de deteccion y signal de presencia
    void motionPixmapGenerator();
    // para usar los metodos de VideoCapture
    VideoCapture mVideoCap;

    // validar idDevice
    static bool isCamIdOk(int idCam){
        cv::VideoCapture testVideoCap(idCam);
        if(!testVideoCap.isOpened()){  // check if we succeeded
            qDebug() << "Id cam: " << idCam << " no admitido.";
            testVideoCap.release();
            return 0;
        }
        testVideoCap.release();
        qDebug() << "Id cam: " << idCam << " OK.";
        return 1;
    }
    void setMin_area_detectar(double value);

signals:
    void newPixmapCapture();
    // para deteccion de presencia
    void presenciaDetectada();
    void sinPresencia();
protected:
    void run();
    double largest_area;
    double min_area_detectar;
    double largest_contour_index;
    Rect bounding_rect;
private:
    QPixmap mPixmap,mPixmapDif,mPixmapDST;
    Mat actFrame, antFrame, difFrame, kernel, dst;
    QImage  cvMatToQImage( const Mat &inMat );
    QPixmap cvMatToQPixmap( const Mat &inMat );
};

#endif // VIDEOMOTION_H
