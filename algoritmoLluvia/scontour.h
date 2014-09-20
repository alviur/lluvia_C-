#ifndef SCONTOUR_H
#define SCONTOUR_H
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;
class Scontour
{
public:
    Scontour();
    ~Scontour();
    static int GetContourMask(const cv::Mat &Mat,std::vector<std::vector<cv::Point> > &contours, int _Threshold,int ContourMode=cv::RETR_LIST, int ContourPixels=cv::CHAIN_APPROX_NONE);
    static int GetContourHierachy(const cv::Mat& Mat,std::vector<std::vector<cv::Point> > &contours,cv::Mat &hierarchy,int _Threshold,int ContourMode, int ContourPixels);
    static int GetBiggestContour(std::vector<vector<Point> > &contours, std::vector<Point> &Big, double Area=0.0d);
    static int AreaBandPass(vector<vector<Point> > &contours, double min, double max);
    static bool widthHeighBandPass(const Mat &a, int widthMin, int widthMax, int heightMin, int heightMax);
    static int GetBiggestFiveContour(std::vector<vector<Point> > &contours,std::vector<vector<Point> > &Big, double Area);
    static cv::Mat drawCont(cv::Mat source,vector<vector<Point> > cmuestra);

};

#endif // SCONTOUR_H
