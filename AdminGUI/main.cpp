#include "admingui.h"
#include <QApplication>
#include "facelogin.h"
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //添加数据库驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名
    db.setDatabaseName("../face.db");
    //打开数据库
    if(!db.open())
    {
        qDebug()<<"open error";
    }
    qDebug()<<"open db success";
    FaceLogin w;
    w.show();

    return a.exec();
}
