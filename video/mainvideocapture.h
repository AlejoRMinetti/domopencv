#ifndef MAINVIDEOCAPTURE_H
#define MAINVIDEOCAPTURE_H
#include <QPixmap>
#include <QImage>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// para debug
#include <QDebug>

//#define ID_CAMERA 0 //"http://192.168.0.20:8081"
#define MAX_ID_CAMS 10


class MainVideoCapture : public QThread
{
    Q_OBJECT
public:
    MainVideoCapture(int idCam, QObject *parent = nullptr );
    //for realise cam
    ~MainVideoCapture(){
        mVideoCap.release();
    }
    // PixMaps getter
    QPixmap getRGBpixmap() const;
    QPixmap getHSVpixmap() const;
    // RGB con detecciones getters
    QPixmap getActualDetect() const;
    QPixmap getAllDetections() const;
    // binario actual getter
    QPixmap getBinarioDetec() const;

    // check for OK of ID Device
    static bool isCamIdOk(int idCam){
        cv::VideoCapture testVideoCap(idCam);
        if(!testVideoCap.isOpened()){  // check if we succeeded
            //qDebug() << "Id cam: " << idCam << " no admitido.";
            testVideoCap.release();
            return 0;
        }
        testVideoCap.release();
        //qDebug() << "Id cam: " << idCam << " OK.";
        return 1;
    }

    // public variables
    ////////// variables de deteccion de objeto
    //Variables to hold HSV values. (HSV = HUE, SATURATION and VALUE)
    int H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX;

    //Struct to define the attributes of the objects to be tracked.
    typedef struct {
        std::string name;
        int x;
        int y;
        int H_MIN= 0;
        int H_MAX = 255;
        int S_MIN= 0;
        int S_MAX= 255;
        int V_MIN= 0;
        int V_MAX= 255;
        cv::Mat threshold;     // binario de deteccion
    }item ;

    // Dynamic object who's attributes (specifically HSV) can be changed in real time.
    item testObject;     // binario de deteccion actual
    // To store objects with defined values.
    std::vector<item> objects;

    // item setter
    item setUpObject(std::string name, int hmin, int hmax, int smin, int smax, int vmin, int vmax);

    bool getPresencia() const;

signals:
    void newPixmapCapture();
    void presenciaDetectada();
    void sinPresencia();

protected:
    void run(); //virtual del Qthread
private:
    // imagenes de captura
    QPixmap RGBpixmap;
    cv::Mat RGBframe; // original
    QPixmap HSVpixmap;
    cv::Mat HSVframe; // procesada HSV
    cv::VideoCapture mVideoCap; // captura de video
    // agregando marcos de deteccion al RGB original
    QPixmap actualDetect;
    cv::Mat actualDetectFrame;
    QPixmap allDetections;
    cv::Mat allDetectionsFrame;
    // binario de deteccion actual
    QPixmap binarioDetec;

    // bandera de presencia
    bool Presencia = false;
    int DetectandoValidar = 3;
    int sinDetecValidar = 10;

    ///////// Configuracion de objetos captura
    // area de captura
    // todo configurable no estatico
    //capture frame width and height
    const int WIDTH = 640;
    const int HEIGHT = 480;
    //max number of objects to be tracked in frame
    static const int MAX_OBJECTS = 20;
    //minimum and maximum object area
    // todo configurable no estatico
    const int MIN_OBJECT_AREA = 25 * 25;
    const int MAX_OBJECT_AREA = HEIGHT*WIDTH / 2;

    // Area and Sides of Threshold Contour
    double area;
    double sideLength;
    // temp variable for position deteted objects
    int x = 0, y = 0;

    /////////// utility for convertion
    QPixmap cvMatToQPixmap( const cv::Mat &inMat );
    QImage cvMatToQImage( const cv::Mat &inMat );
    // for edge detection
    std::string numberToString(int number);
    void drawObject(int x, int y, cv::Mat &frame, item tempItem);
    void morphObject(cv::Mat &thresh);
    bool trackObject(int &x, int &y, item tempItem, cv::Mat &cameraFeed);
    // comprueba nuevo evento de deteccion y envia signal, corre en cada imagen
    void signalDetection(bool prese);
};

#endif // MAINVIDEOCAPTURE_H
