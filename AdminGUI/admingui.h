#ifndef ADMINGUI_H
#define ADMINGUI_H

#include <QMainWindow>
#include "inputfacethread.h"
#include "facetrainthread.h"
#include <QProcess>

namespace Ui {
class AdminGUI;
}

class AdminGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminGUI(QWidget *parent = nullptr);
    ~AdminGUI();

private slots:
    void on_inputInfo_triggered();
    void on_processInfo_triggered();
    void on_trainInfo_triggered();
    void showImage(cv::Mat mat);
    void on_showTable_triggered();
    void on_inputinfo_triggered();
    void on_attFace_triggered();
    void fTrain();
    void fInput();

private:
    Ui::AdminGUI *ui;
    FaceTrainThread *facetrainTh;
    InputFaceThread *inputFaceTh;
    QProcess mprocess;
};

#endif // ADMINGUI_H
