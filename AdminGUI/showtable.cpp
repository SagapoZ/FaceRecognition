#include "showtable.h"
#include "ui_showtable.h"
#include "admingui.h"
#include <QSqlTableModel>

ShowTable::ShowTable(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ShowTable)
{
    ui->setupUi(this);
    //绑定模型
    smodel.setTable("user"); //给模型绑定数据表格
    ui->tableView->setModel(&smodel);//把模型与显示view绑定
    smodel.select();
}

ShowTable::~ShowTable()
{
    delete ui;
}

void ShowTable::on_backBt_clicked()
{
    //返回主界面
    this->parentWidget()->show();
    this->close();
}
