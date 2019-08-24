#include "facelogin.h"
#include "ui_facelogin.h"
#include "admingui.h"
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <fstream>
#include <sstream>
#include <math.h>
#include "face.hpp"
#include <QTimer>
#include <QDebug>
#include <QSqlRecord>
#include <QMessageBox>
#include <QMap>
#include <QDateTime>
#include <QSqlQuery>
#include <QPainter>
using namespace std;
using namespace cv;

RNG FaceLogin::g_rng(12345);

FaceLogin::FaceLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaceLogin)
{
    ui->setupUi(this);
    //设置当前时间
    QDateTime current_date_time =QDateTime::currentDateTime();
    current_ymd = current_date_time.toString("yyyy.MM.dd ddd");
    //绑定数据模型
    userlabel = -1;
    umodel.setTable("user");//给模型绑定数据表格
    //显示当前事件
    connect(&time,SIGNAL(timeout()),this,SLOT(updatetime()));
    time.start(1000);
}
void FaceLogin::on_recfaceBt_clicked()
{
    //人脸识别
    //打开默认摄像头
    cap = cv::VideoCapture(0);
    if (!cap.isOpened())
    {
        cout<<"open failed"<<endl;
        return;
    }
    qDebug()<<"open success"<<endl;
    //训练好的文件名称
    cascade.load("../AdminGUI/haarcascade_frontalface_alt2.xml");
    model = face::FisherFaceRecognizer::create();
    //1.加载训练好的分类器
    model->read("../AdminGUI/datamodel/MyFaceFisherModel.xml");// opencv2用load

    //开启定时器
    connect(&mtime,SIGNAL(timeout()),this,SLOT(faceRecog()));
    mtime.start(25);
}

FaceLogin::~FaceLogin()
{
    delete ui;
    mtime.stop();
}
void FaceLogin::updatetime()
{
    //更新当前事件到界面上
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("hh:mm:ss");
    ui->hmslabel->setText(current_date);

}
static int a=0;
static int num=0;
static bool isSelect = false;
void FaceLogin::faceRecog()
{
    cap >> frame;

    vector<Rect> faces(0);//建立用于存放人脸的向量容器

    cvtColor(frame, gray, CV_RGB2GRAY);//测试图像必须为灰度图

    equalizeHist(gray, gray); //变换后的图像进行直方图均值化处理
    //检测人脸
    cascade.detectMultiScale(gray, faces,
        1.1, 4, 0
        | CV_HAAR_DO_ROUGH_SEARCH,
        Size(30, 30), Size(500, 500));
    Mat* pImage_roi = new Mat[faces.size()];    //定以数组
    Mat face;
    Point text_lb;//文本写在的位置
    QString name;
    if(faces.size() == 1)
    {
        pImage_roi[0] = gray(faces[0]); //将所有的脸部保存起来
        text_lb = Point(faces[0].x, faces[0].y);
        userlabel = Predict(pImage_roi[0]);
        qDebug()<<userlabel;
        //做几次对比，当检查五次都是这个人就打卡
        if(a == userlabel)
        {
            num++;
            if(num > 5)
            {
                isSelect = true;
            }
        }
        else {
            a = userlabel;
            num=0;
        }
        if(isSelect)
        {
            //进行数据库查询,查询谁登录
            umodel.setFilter(QString("label=%1").arg(userlabel));
            umodel.select();
            int row = umodel.rowCount();//保存查询的行数
            if(row > 0)
            {
                //找到了该用户,查找成功
                QSqlRecord record = umodel.record(0);
                name = record.value(1).toString();
                int attencenum = record.value(5).toInt();
                QString attTime = record.value(6).toString();  //打开时间
                qDebug()<<name<<" "<<attencenum<<" "<<attTime;
                //开始打卡
                ui->namelabel->setText(name);
                ui->numlabel->setText(QString::number(userlabel));
                if(current_ymd != attTime)
                {
                 attTime = current_ymd;
                 faceSuccess(name,attencenum,attTime);
                }
            }
            num=0;
            isSelect = false;
        }
        rectangle(frame, Point(faces[0].x, faces[0].y), Point(faces[0].x + faces[0].width, faces[0].y + faces[0].height), Scalar(241,241,241), 1, 8);//放入缓存
    }
    delete[]pImage_roi;
    setIamge(frame);
}

void FaceLogin::faceSuccess(QString name,int attencenum, QString attTime)
{
    //更新数据库
    QString sql = QString("update user set attence=%1,attencetime='%2' where name='%3';")
            .arg(attencenum+1)
            .arg(attTime)
            .arg(name);
    QSqlQuery query(sql);
    if(!query.exec())
    {
        qDebug()<<"exec error";
    }
    ui->attlabel->setText(attTime);
    QMessageBox::about(NULL,"打卡",attTime+"\n"+name+": 打卡成功");
}

void FaceLogin::setIamge(Mat mat)
{
    //显示图像
    Mat Rgb;
    cv::cvtColor(mat, Rgb, CV_BGR2RGB);//颜色空间转换
    img = QImage((const uchar*)(Rgb.data),
                 Rgb.cols,
                 Rgb.rows,
                 Rgb.cols * Rgb.channels(),
                 QImage::Format_RGB888);
    QPixmap src = QPixmap::fromImage(img);
    ui->facelabel->setPixmap(src);
    ui->facelabel->show();
}

int FaceLogin::Predict(Mat src_image)  //识别图片
{
    Mat face_test;
    int predict = -1;
    //截取的ROI人脸尺寸调整
    if (src_image.rows >= 120)
    {
        //改变图像大小，使用双线性差值
        cv::resize(src_image, face_test, Size(92, 112));
    }
    //判断是否正确检测ROI
    if (!face_test.empty())
    {
        //测试图像应该是灰度图
        predict = model->predict(face_test);
    }
    //cout << predict << endl;
    return predict;
}

void FaceLogin::on_adminBt_clicked()
{
    //进入管理员界面
    AdminGUI *a = new AdminGUI(this);
    a->show();
    this->hide();
    cap.release();
    ui->facelabel->clear();
    mtime.stop();
}




