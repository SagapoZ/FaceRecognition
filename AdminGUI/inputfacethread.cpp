#include "inputfacethread.h"
#include <QThread>
#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <stdio.h>
#include <QDebug>
#include <QDir>
#include <QMetaType>
#include <QTextCodec>

using namespace std;
using namespace cv;

InputFaceThread::InputFaceThread(QString username,QObject *parent)
    :QThread (parent),
     username(username)
{
    dirpath = QString("../video/att_faces/%1").arg(username);
    QDir dir;
    if(!dir.exists(dirpath))
    {
        //创建存储照片的文件夹
        dir.mkdir(dirpath);
    }
    num=0;
}

InputFaceThread::~InputFaceThread()
{

}
static bool isSleep = true;
void InputFaceThread::run()
{
    //收集人脸数据
    CascadeClassifier cascada;
    //将opencv官方训练好的人脸识别分类器拷贝到自己的工程目录中
    cascada.load("../AdminGUI/haarcascade_frontalface_alt2.xml");
    VideoCapture cap(0);
    Mat frame, myFace;
    int pic_num = 1;
    while (1) {
        //摄像头读图像
        cap >> frame;
        vector<Rect> faces;//vector容器存检测到的faces
        Mat frame_gray;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);//转灰度化，减少运算
        cascada.detectMultiScale(frame_gray, faces, 1.1, 4, CV_HAAR_DO_ROUGH_SEARCH, Size(70, 70), Size(1000, 1000));
        qDebug()<<"check face size: "<<faces.size();
        //识别到的脸用矩形圈出
        if(num <= 10)
        {
            for (size_t i = 0; i < faces.size(); i++)
            {
                rectangle(frame, faces[i], Scalar(255, 0, 0), 2, 8, 0);
            }

            emit setImage(frame);
            msleep(200);
            num++;
        }

        if(num > 10)
        {
            //当只有一个人脸时，开始拍照
            if (faces.size() == 1 && num>10)
            {
                Mat faceROI = frame_gray(faces[0]);//在灰度图中将圈出的脸所在区域裁剪出
                resize(faceROI, myFace, Size(92, 112));//将兴趣域size为92*112
                putText(frame, to_string(pic_num), faces[0].tl(), 3, 1.2, (0, 0, 225), 2, 0);//在 faces[0].tl()的左上角上面写序号
                QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
                string name = code->fromUnicode(username).data();
                string filename = format("../video/att_faces/%s/%d.jpg",name.c_str(),pic_num); //存放在当前项目文件夹以1-10.jpg 命名，format就是转为字符串
                imwrite(filename, myFace);//存在当前目录下
                imshow(filename, myFace);//显示下size后的脸
                waitKey(500);//等待500us
                destroyWindow(filename);//:销毁指定的窗口
                pic_num++;//序号加1
                if (pic_num == 21)
                {
                    emit finished();
                    return;//当序号为20时退出循环,一共拍20张照片
                }
            }
            num++;
            //识别到的脸用矩形圈出
            for (size_t i = 0; i < faces.size(); i++)
            {
                rectangle(frame, faces[i], Scalar(255, 0, 0), 2, 8, 0);
            }
            emit setImage(frame);
            msleep(200);
        }
    }
}
