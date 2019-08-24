#ifndef FACETRAINTHREAD_H
#define FACETRAINTHREAD_H
#include <QThread>
#include "face.hpp"
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;
using namespace cv;

class FaceTrainThread:public QThread
{
    Q_OBJECT
public:
    explicit FaceTrainThread(QObject *parent = nullptr);
    ~FaceTrainThread();
    static Mat norm_0_255(InputArray _src);
    static void read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator = ';');

signals:
    void finished();

public:
    void run();

};

#endif // FACETRAINTHREAD_H
