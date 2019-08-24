#ifndef INPUTFACETHREAD_H
#define INPUTFACETHREAD_H
#include <QThread>
#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class InputFaceThread:public QThread
{
    Q_OBJECT
public:
    explicit InputFaceThread(QString username,QObject *parent = nullptr);
    ~InputFaceThread();

public:
    void run();

signals:
    void setImage(cv::Mat mat);
    void finished();

private:
    QString username;
    QString dirpath;
    int num;
};

#endif // INPUTFACETHREAD_H
