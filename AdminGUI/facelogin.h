#ifndef FACELOGIN_H
#define FACELOGIN_H

#include <QWidget>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <fstream>
#include <sstream>
#include <math.h>
#include "face.hpp"
#include <QTimer>
#include <QSqlTableModel>
#include <QMouseEvent>

using namespace std;
using namespace cv;

namespace Ui {
class FaceLogin;
}

class FaceLogin : public QWidget
{
    Q_OBJECT

public:
    explicit FaceLogin(QWidget *parent = nullptr);
    ~FaceLogin();
    void setIamge(Mat mat);

    int Predict(Mat src_image);
    void faceSuccess(QString name,int attencenum,QString attTime);

private slots:
    void on_adminBt_clicked();
    void faceRecog();
    void updatetime();

    void on_recfaceBt_clicked();

private:
    Ui::FaceLogin *ui;
    QImage img;
    static RNG g_rng;
    Ptr<face::FaceRecognizer> model;
    VideoCapture cap;
    Mat frame;
    Mat gray;
    //这个分类器是人脸检测所用
    CascadeClassifier cascade;
    QTimer mtime;
    int userlabel;
    QSqlTableModel umodel;
    QTimer time;
    QString current_ymd;

};

#endif // FACELOGIN_H
