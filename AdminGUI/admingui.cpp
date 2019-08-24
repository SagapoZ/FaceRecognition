#include "admingui.h"
#include "ui_admingui.h"
#include "inputfacethread.h"
#include "facetrainthread.h"
#include <QThread>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <showtable.h>
#include <QPainter>
#include "facelogin.h"
#include <QTextCodec>

static bool isFtrain = false;
static bool isFinput = false;

AdminGUI::AdminGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminGUI)
{
    ui->setupUi(this);
    qRegisterMetaType<cv::Mat>("cv::Mat");
}

AdminGUI::~AdminGUI()
{
    qDebug()<<"delete";
    delete ui;
    if(isFtrain)
    {
        qDebug()<<"facetrainTh";
        delete facetrainTh;
    }
    if(isFinput)
    {
        qDebug()<<"inputFaceTh";
        delete inputFaceTh;
    }
}

void AdminGUI::on_inputInfo_triggered()
{
    //开启线程开始采集
    QString username = ui->nameEdit->text();
    if(username.isEmpty())
    {
         QMessageBox::about(this,"信息","<font size='1' color='black'>请输入员工信息</font>");
    }
    inputFaceTh = new InputFaceThread(username);
    connect(inputFaceTh,&QThread::finished,this,&AdminGUI::fInput);
    inputFaceTh->start();
    connect(inputFaceTh,&InputFaceThread::setImage,this,&AdminGUI::showImage);
}
void AdminGUI::fInput()
{
    //完成采集
    isFinput=true;
    ui->showlabel->clear();
    QMessageBox::about(this,"采集图片","<font size='1' color='black'>采集成功</font>");
}
void AdminGUI::on_processInfo_triggered()
{
    //处理图片
    QMessageBox::about(this,"处理图像","<font size='1' color='black'>处理成功</font>");
    mprocess.start("python ../AdminGUI/ProcessFile.py");

}

void AdminGUI::on_trainInfo_triggered()
{
    //开始训练模型，开启一个线程进行训练
    facetrainTh = new FaceTrainThread();
    //线程结束销毁线程
    connect(facetrainTh,&QThread::finished,this,&AdminGUI::fTrain);
    facetrainTh->start();
}
void AdminGUI::fTrain()
{
    //完成训练
    isFtrain = true;

    QMessageBox::about(this,"训练数据","<font size='1' color='black'>训练成功</font>");
}
void AdminGUI::showImage(cv::Mat mat)
{
    //显示线程发过来的图像
    Mat Rgb;
    cv::cvtColor(mat, Rgb, CV_BGR2RGB);//颜色空间转换
    QImage img = QImage((const uchar*)(Rgb.data),
                 Rgb.cols, Rgb.rows,
                 Rgb.cols * Rgb.channels(),
                 QImage::Format_RGB888);
    ui->showlabel->setPixmap(QPixmap::fromImage(img));
    ui->showlabel->show();
}

void AdminGUI::on_showTable_triggered()
{
    //查表
    ShowTable *t = new ShowTable(this);
    t->show();
    this->hide();
}

void AdminGUI::on_inputinfo_triggered()
{
    //录入员工信息
    QString sql = QString("update user set sex='%1',age=%2,tel='%3' where name='%4';")
                        .arg(ui->sexEdit->text())
                        .arg(ui->ageEdit->text())
                        .arg(ui->telEdit->text())
                        .arg(ui->nameEdit->text());
    QSqlQuery query(sql);
    if(!query.exec())
    {
        QMessageBox::about(this,tr("录入信息"),tr("<font size='1' color='black'>录入员工信息失败</font>"));
    }
    else
    {
        QMessageBox::about(this,tr("录入信息"),"<font size='1' color='black'>录入员信息成功</font>");
    }

}

void AdminGUI::on_attFace_triggered()
{
    //创建打卡界面
    this->parentWidget()->show();
    delete this;
}
