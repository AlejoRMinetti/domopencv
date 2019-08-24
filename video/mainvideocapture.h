#ifndef MAINVIDEOCAPTURE_H
#define MAINVIDEOCAPTURE_H
#include <QPixmap>
#include <QImage>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define ID_CAMERA 0 //"http://192.168.0.20:8081"

class MainVideoCapture : public QThread
{
    Q_OBJECT
public:
    MainVideoCapture(QObject *parent = nullptr );
    QPixmap pixmap() const{
        return mPixmap;
    }
signals:
    void newPixmapCapture();
protected:
    void run();
private:
    QPixmap mPixmap;
    cv::Mat mFrame; // original
    cv::Mat imagen_procesada; // procesada
    cv::VideoCapture mVideoCap;
    QPixmap cvMatToQPixmap( const cv::Mat &inMat );
    QImage cvMatToQImage( const cv::Mat &inMat );
};

#endif // MAINVIDEOCAPTURE_H
