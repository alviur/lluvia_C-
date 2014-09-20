#include "scontour.h"

Scontour::Scontour()
{
}

Scontour::~Scontour()
{
}

/***************************************************************************************************
Metodos
Autor: Alexander Gómez villa - German Diez Valencia
Descripcion: Encuentra la intensidad mayor en la imagen  la coloca en inten,retorna código de error.

***************************************************************************************************/

int Scontour::GetContourMask(const cv::Mat &Mat,std::vector<std::vector<cv::Point> > &contours, int _Threshold,int ContourMode, int ContourPixels)
{
    /* 0: Binary
       1: Binary Inverted
       2: Threshold Truncated
       3: Threshold to Zero
       4: Threshold to Zero Inverted
     */
    cv::Mat dst;
    cv::threshold(Mat,dst,_Threshold,255,cv::THRESH_OTSU);



    cv::findContours(   dst,
                        contours, // a vector of contours
                        cv::RETR_LIST, // retrieve the external contours
                        ContourPixels); // all pixels of each contours
    return 1;
}

/***************************************************************************************************
Metodos
Autor: Alexander Gómez villa - German Diez Valencia
Descripcion: Encuentra la intensidad mayor en la imagen  la coloca en inten,retorna código de error.

***************************************************************************************************/

int Scontour::GetContourHierachy(const cv::Mat& Mat,std::vector<std::vector<cv::Point> > &contours,cv::Mat &hierarchy,int _Threshold,int ContourMode, int ContourPixels)
{
    /* 0: Binary
       1: Binary Inverted
       2: Threshold Truncated
       3: Threshold to Zero
       4: Threshold to Zero Inverted
     */
    cv::Mat dst;
    cv::threshold(Mat,dst,_Threshold,255,cv::THRESH_OTSU);


    cv::findContours(   dst,
                        contours, // a vector of contours
                        hierarchy,
                        cv::RETR_TREE, // retrieve the external contours
                        ContourPixels); // all pixels of each contours
    return 1;
}



/***************************************************************************************************
Metodos
Autor: Alexander Gómez villa - German Diez Valencia
Descripcion: Encuentra la intensidad mayor en la imagen  la coloca en inten,retorna código de error.

***************************************************************************************************/

int Scontour::GetBiggestContour(std::vector<vector<Point> > &contours, std::vector<Point> &Big, double Area)
{

    //std::cout<<"tamaño: "<<contours.size();
    int keeper=0;
    for(int index=0;index<(int)contours.size();index++)
    {
        if(cv::contourArea(contours[index])>Area)
        {
            Area=cv::contourArea(contours[index]);//calcula el area del contorno
            keeper=index;//captura el indice del contorno de interes
        }
    }
    Big=contours[keeper];
    //cout<<cv::contourArea(Biggest)<<"....."<<endl;
    return 1;
}


/***************************************************************************************************
Metodos

Autor: Alexander Gómez villa  - German Diez Valencia
Descripcion: Encuentra los 5 ocntornos mas grandes.

***************************************************************************************************/

int Scontour::GetBiggestFiveContour(std::vector<vector<Point> > &contours,std::vector<vector<Point> > &Big, double Area)
{

    //std::cout<<"tamaño: "<<contours.size();
    int num=0;
    for(int index=0;index<(int)contours.size();index++)
    {
        if(cv::contourArea(contours[index])>Area)
        {
            Area=cv::contourArea(contours[index]);//calcula el area del contorno
     	   // Big[num]=contours[index];
	    num++;
	    if(num>4)num=0;		
        }
    }
    
    //cout<<cv::contourArea(Biggest)<<"....."<<endl;
    return 1;
}

/***************************************************************************************************
Metodos
Autor: Alexander Gómez villa - German Diez Valencia
Descripcion: Encuentra la intensidad mayor en la imagen  la coloca en inten,retorna código de error.

***************************************************************************************************/

int Scontour::AreaBandPass(vector<vector<Point> > &contours, double min, double max)
{
    std::vector<std::vector< cv::Point> >::iterator itc= contours.begin();
    while (itc!=contours.end()) {
    if (itc->size() < min || itc->size() > max)//
    itc = contours.erase(itc);
    else
    ++itc;
    }
    return 1;
}



bool Scontour::widthHeighBandPass(const cv::Mat &a, int widthMin, int widthMax , int heightMin, int heightMax)
{

    if(a.cols>widthMin && a.cols<widthMax)
    {
        if(a.rows>heightMin && a.rows<heightMax)
        {
            return true;
        }
    }
    else
    {
        return false;
    }
    return false;

}

/**************************************************************************************************
Funciones
Autor: Alexander Gómez villa - German Diez Valencia
Descripcion:Dibuja los contornos
***************************************************************************************************/

cv::Mat Scontour::drawCont(cv::Mat source,vector<vector<Point> > cmuestra)
{
    cv::Mat src;
    source.copyTo(src);
    for(int i=0;i < cmuestra.size()-1;i++)
    {
                cv::drawContours(src,cmuestra,i,Scalar(0,0,255),2,8);

    }
    return src;
}
