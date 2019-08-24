#ifndef SHOWTABLE_H
#define SHOWTABLE_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class ShowTable;
}

class ShowTable : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShowTable(QWidget *parent = nullptr);
    ~ShowTable();

private slots:
    void on_backBt_clicked();

private:
    Ui::ShowTable *ui;
    QSqlTableModel smodel;
};

#endif // SHOWTABLE_H
