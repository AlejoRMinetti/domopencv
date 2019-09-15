#include "video/mainvideocapture.h"
#include <QDebug>

using namespace cv;
using namespace std;

MainVideoCapture::MainVideoCapture(QObject *parent)
    : QThread { parent }, mVideoCap { ID_CAMERA }
{}

void MainVideoCapture::run()
{
    if (mVideoCap.isOpened()){
        while (true) {
            // procesando imagen
            //Canny( RGBframe, imagen_procesada, 100, 200 ); // edge detector
            // pocesar imagen de video y emitir senial
            mVideoCap >> RGBframe;
            if(!RGBframe.empty()){
                // RGB frame a pixmap
                RGBpixmap = cvMatToQPixmap(RGBframe);

                // para posterior conversion
                actualDetectFrame = RGBframe;
                allDetectionsFrame = RGBframe;

                //Convert the captured frame from BGR to HSV
                cvtColor(RGBframe,HSVframe, COLOR_BGR2HSV);
                HSVpixmap = cvMatToQPixmap(HSVframe);

                /////////// creacion de imagenes binarias de deteccion
                //creacion de la img binaria testObject.threshold segun los parametros seteados
                inRange(HSVframe, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), testObject.threshold);
                // texto de objeto actual
                testObject.name="Detectado";

                // obtiene binario de deteccion
                morphObject(testObject.threshold);
                // convert a pixmap
                binarioDetec = cvMatToQPixmap(testObject.threshold);

                // agregar tracking del objeto detectado al frame
                trackObject(x, y, testObject, actualDetectFrame);

                actualDetect = cvMatToQPixmap(actualDetectFrame);

                // lo mismo para todos los objetos agregados para deteccion
                for (unsigned int i = 0; i<objects.size(); i++) {
                    inRange(HSVframe, Scalar(objects[i].H_MIN, objects[i].S_MIN, objects[i].V_MIN), Scalar(objects[i].H_MAX, objects[i].S_MAX, objects[i].V_MAX), objects[i].threshold);
                    morphObject(objects[i].threshold);
                    trackObject(x, y, objects[i], allDetectionsFrame);
                }
                allDetections = cvMatToQPixmap(allDetectionsFrame);

                // señal de nueva imagen
                emit newPixmapCapture();
            }
        }
    }
}

QPixmap MainVideoCapture::getBinarioDetec() const
{
    return binarioDetec;
}

QPixmap MainVideoCapture::getAllDetections() const
{
    return allDetections;
}

QPixmap MainVideoCapture::getActualDetect() const
{
    return actualDetect;
}

////////// QpixMap getters
QPixmap MainVideoCapture::getRGBpixmap() const{
    return RGBpixmap;
}

QPixmap MainVideoCapture::getHSVpixmap() const
{
    return HSVpixmap;
}

////////// Frame to pixmaps
QImage MainVideoCapture::cvMatToQImage( const cv::Mat &inMat )
{

    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_ARGB32 );

        return image;
    }

    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
    #if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
            QImage image( inMat.data,
                        inMat.cols, inMat.rows,
                        static_cast<int>(inMat.step),
                        QImage::Format_Grayscale8 );
    #else
            static QVector<QRgb>  sColorTable;

            // only create our color table the first time
            if ( sColorTable.isEmpty() )
            {
                sColorTable.resize( 256 );

                for ( int i = 0; i < 256; ++i )
                {
                    sColorTable[i] = qRgb( i, i, i );
                }
            }

            QImage image( inMat.data,
                        inMat.cols, inMat.rows,
                        static_cast<int>(inMat.step),
                        QImage::Format_Indexed8 );

            image.setColorTable( sColorTable );
    #endif

        return image;
    }

    default:
        qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}

QPixmap MainVideoCapture::cvMatToQPixmap( const cv::Mat &inMat )
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

// numero a String
string MainVideoCapture::numberToString(int number){
    std::stringstream ss;
    ss << number;
    return ss.str();
}

// dibuja marco detectado con atributos
void MainVideoCapture::drawObject(int x, int y, Mat &frame, item tempItem){
    rectangle(frame,Point(x-(sideLength/2),y-(sideLength/2)), Point(x+(sideLength/2),y+(sideLength/2)),Scalar(0,255,0),2);
    putText(frame, numberToString(x) + "," + numberToString(y), Point(x, y + 30), 1, 1, Scalar(0,72, 255), 1);
    putText(frame, tempItem.name , Point(x, y + 50), 2, 1, Scalar(0,72, 255), 1);
}

// convertir deteccion
void MainVideoCapture::morphObject(Mat &thresh){
    // todo entender que pasa y crear pixmap para configurar detecciones

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

    erode(thresh, thresh, erodeElement);
    erode(thresh, thresh, erodeElement);
    dilate(thresh, thresh, dilateElement);
    dilate(thresh, thresh, dilateElement);

}

// agregar tracking del objeto detectado al frame
void MainVideoCapture::trackObject(int &x, int &y, item tempItem, Mat &cameraFeed){

    Mat temp;
    tempItem.threshold.copyTo(temp);
    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    if (hierarchy.size() > 0) {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_OBJECTS we have a noisy filter
        if (numObjects<MAX_OBJECTS){
            for (int index = 0; index >= 0; index = hierarchy[index][0]) {

                Moments moment = moments((cv::Mat)contours[index]);
                area = moment.m00;
                sideLength= sqrt(area);

                //if the area is less than 25 px by 25px then it is probably just noise
                //if the area is the same as the 1/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                if (area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
                    x = moment.m10 / area;
                    y = moment.m01 / area;
                    objectFound = true;
                    refArea = area;
                }
                else objectFound = false;


            }
            //let user know you found an object
            if (objectFound == true){
                //draw object location on screen
                drawObject(x, y, cameraFeed, tempItem);
            }
        }
    }
}

// setter de objetos de deteccion
MainVideoCapture::item MainVideoCapture::setUpObject(string name, int hmin, int hmax, int smin, int smax, int vmin, int vmax){

    item temp;
    temp.name = name;
    //temp.x = x;
    //temp.y = y;
    temp.H_MIN = hmin;
    temp.H_MAX = hmax;
    temp.S_MIN = smin;
    temp.S_MAX = smax;
    temp.V_MIN = vmin;
    temp.V_MAX = vmax;

    return temp;
}
