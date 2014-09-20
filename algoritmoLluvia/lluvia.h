#ifndef LLUVIA_H
#define LLUVIA_H

#include <QMainWindow>
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <QString>
#include <QDebug>
#include<QFileDialog>
#include<qtimer.h>
#include "scontour.h"
#include <math.h>

using namespace std;
using namespace cv;

namespace Ui {
class lluvia;
}

class lluvia : public QMainWindow
{
    Q_OBJECT

public:
    explicit lluvia(QWidget *parent = 0);
    ~lluvia();


    //PATHS
    string inputName;

    //flags
    bool play;

    cv::VideoCapture video;

    //Frame update
     QTimer* tmrTimer;//interuption
     int     FPS;//frames per second


     //images
    cv::Mat frame;
    cv::Mat frameRGB;
    cv::Mat frameGray;
    cv::Mat frameSobel;
    cv::Mat img_frameAnt;
    cv::Mat img_sub;
    cv::Mat img_subGray;

    //Buffers

    std::vector<cv::Mat> buff_main;
    std::vector<cv::Mat> buff_rain;
    std::vector<cv::Mat> buff_gray;


    //counters
    int cont_buffPosition;

    //contours
    std::vector<std::vector<cv::Point> > contours;
    double cont_perimeter;
    double perimeterMin;


public slots:
 void  frameUpdate();


private slots:
    void on_pushButton_released();

    void on_pushButton_3_released();

    void on_pushButton_2_released();

private:
    Ui::lluvia *ui;
};

#endif // LLUVIA_H
