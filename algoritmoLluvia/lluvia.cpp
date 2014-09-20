#include "lluvia.h"
#include "ui_lluvia.h"

lluvia::lluvia(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::lluvia)
{
    ui->setupUi(this);
    //interruption time
      FPS=33;//time of interruption in mS
      QTimer *timer = new QTimer(this);//Qt object
      connect(timer, SIGNAL(timeout()), this, SLOT(frameUpdate()));

      //set video
      if(video.isOpened())//if camera is opened
        video.release();
      video.open(inputName);
      timer->start(33);
      play=false;

     //set Counters
      cont_buffPosition=0;


      //buffer initialization

      std::vector<cv::Mat> buff(10);
      buff_main= buff;


      cont_perimeter=0;
      perimeterMin=0.7;

}

lluvia::~lluvia()
{
    delete ui;
}


/***************************************************************************************************
Metodos
Autor:   Alexander Gómez villa - German Diez Valencia - Sebastian Guzman Obando
Descripcion:

***************************************************************************************************/

cv::Mat regionGrowing(cv::Mat src,cv::Mat mask,std::vector<cv::Mat> buffer)
{
    cv::Mat img_media;
    cv::Mat img_varianza;
    cv::Mat salida;

    src.copyTo(img_media);
    src.copyTo(img_varianza);
    src.copyTo(salida);
    int int_intensidad=0;


    ///calculo de pixeles anomalos
    //calculo de la media-criterio de union

    for(int j=0;j<src.rows-1;j++)
    {
        for(int i=0;i<src.cols-1;i++)
        {
            for(int k=0;k<buffer.size()-1;k++)
            {
                int_intensidad+= buffer[k].at<int>(i,j);
            }

            img_media.at<int>(i,j,0)=int_intensidad/buffer.size();
            int_intensidad=0;
        }

    }

    //calculo de la varianza
    for(int j=0;j<src.rows-1;j++)
    {
        for(int i=0;i<src.cols-1;i++)
        {
            for(int k=0;k<buffer.size()-1;k++)
            {
                int_intensidad+= pow(buffer[k].at<int>(i,j)-img_media.at<int>(i,j),2);
            }

            img_varianza.at<int>(i,j,0)=int_intensidad/buffer.size();
            int_intensidad=0;
        }

    }


    //Region Growing

    int l=0;
    int m=0;

    int region[3]={-1,0,1};


    for(int j=0;j<src.rows-1;j++)
    {
        for(int i=0;i<src.cols-1;i++)
        {
            if(mask.at<int>(i,j)>200)
            {
                while(l<2)
                {
                    while(m<2)
                    {

                        if((i+region[l]<src.cols-1) && (i+region[l]>0)&& (j+region[m]>0) && (j+region[m]<src.rows))//verifica que no se salga de las dimenciones
                        {

                            if(src.at<int>(i+region[l],j+region[m])> img_media.at<int>(i+region[l],j+region[m]+img_varianza.at<int>(i,j,0)*2)||(src.at<int>(i+region[l],j+region[m])< img_media.at<int>(i+region[l],j+region[m])-img_varianza.at<int>(i,j,0)*2))
                            {
                                salida.at<int>(i+region[l],j+region[m],0)=255;//si la intencidad esta 2 desviaciones estandar por fuera

                            }
                        }

                    }
                    l++;
                    m=0;
                }
                l=0;

            }

        }

    }




}



/***************************************************************************************************
Metodos
Autor:   Alexander Gómez villa - German Diez Valencia - Sebastian Guzman Obando
Descripcion:si encuentra una semilla cuyo valor este entre lowThreshold y highThreshold pondra los
pixeles vecinos en el valor SetValue si este vecino cumple ser mayor que condition

***************************************************************************************************/

cv::Mat  gSetMaskSeed2(cv::Mat src,cv::Mat gray ,int lowThreshold, int highThreshold, int SetValue, int condition){


cv::Mat src2;
cv::Mat image;
src.copyTo(src2);
src.copyTo(image);


float valor=0;
    cv::Scalar intensidad;

    cv::Scalar intensidad1;
    intensidad1.val[0] =SetValue;
    int i=0,j=0,pix=0,b=-1;
    int a=0;
    int c=0;
    int x=0;
    int y=0;
    int contpix=0;
    int contpix2=0;

    int pixa[3]={-1,0,1};

    while(i < (450)){
    while(j < (320)){

            intensidad=image.at<int>(j,i);
            valor = intensidad.val[0] ;

            if((valor < highThreshold) && (valor > lowThreshold)){
            x=i;
            y=j;
            while(a<2){
                while(b<2){

                    if(((j+b) <320) && ((i+a) < 450) && ((j+b) >0) && ((i+a) > 0)) {
                        //intensidad = cvGet2D(src,(j+b,i+a);
                        intensidad=  gray.at<int>(j+b,i+a);

                        if((intensidad.val[0] > condition) ){
                            //cvSet2D( src2, j+b,i+a,intensidad1);
                            //src2=src2.at<int>(j+b,i+a);
                            while(contpix2<3){
                                while(contpix<3){
                                    if((intensidad.val[0] > condition) && ((j+ pixa[contpix]) <320)&&(gray.at<int>(j+ pixa[contpix],i+ pixa[contpix2])>condition) && ((i+ pixa[contpix2]) < 450) && ((j+ pixa[contpix]) >0) && ((i+ pixa[contpix2]) > 0)){
                                    //cvSet2D( src2, j+ pixa[contpix],i,intensidad1);
                                    src2.at<int>( j+ pixa[contpix],i,0)=255;
                                    imshow("en proceso",src2);
                                    waitKey(0);
                                    }
                                contpix++;
                                }
                            contpix=0;
                            contpix2++;
                            }


                            contpix2=0;
                            i=i+a;
                            j=j+b;

b++;

                        }
                        else{
                            b++;
                        }
                                }

                    else{
                        b++;

                    }


                }
c=0;
//printf("aca estoy \n");
            a++;
            b=-1;
            i=x;
            j=y;

            }
                 a=-1;

            }


            j++;
        }
    j=0;
    i++;
    }



  return src2;

}




void lluvia::frameUpdate()
{
    if(play)
    {

        if(frame.data)frame.copyTo(img_frameAnt);//copy current to past frame

        video>>frame;//get image form video capture
        cv::cvtColor(frame,frameRGB,4);//change BGR to RGB for QT
        cv::cvtColor(frame,frameGray,cv::COLOR_BGR2GRAY);//change BRG to GRAY

        //MUESTRA IMAGEN EN INTERFAZ
        QImage img= QImage((const unsigned char*)(frameRGB.data),frameRGB.cols,frameRGB.rows,frameRGB.step,QImage::Format_RGB888);//extrae imagen
        ui->label->setPixmap(QPixmap::fromImage(img));//muestra en label el video


        //buffer update
        frame.copyTo(buff_main[cont_buffPosition]);
        cont_buffPosition++;
        if(cont_buffPosition==buff_main.size() -1)cont_buffPosition=0;


        //frame subs
        if(img_frameAnt.data)
        {
            img_sub=(frame -img_frameAnt);
            cvtColor(img_sub,img_subGray,cv::COLOR_BGR2GRAY);
            imshow("resta",img_sub);


        //Filter operations
        cv::Sobel(img_subGray,frameSobel,frameGray.depth(),1,0,3,1,0, BORDER_DEFAULT);

        cv::medianBlur(frameSobel,frameSobel,5);//elimina ruido sal y pimienta

        cv::threshold(frameSobel,frameSobel,20,255,cv::THRESH_BINARY);
        dilate(frameSobel, frameSobel, Mat(), Point(-1, -1), 1, 1, 1);
        imshow("dilate",frameSobel);
        


         Scontour::GetContourMask(frameSobel,contours,30);//Get contours form image



        for(int i=0;i<contours.size()-1;i++)
        {
            cont_perimeter=cv::arcLength(contours[i],-1);
            qDebug()<<cont_perimeter;
            if(cont_perimeter>perimeterMin)
            {

                cv::drawContours(frameSobel,contours,i,Scalar(255,255,255),1,8);
            }

        }


        frameGray=frameGray + frameSobel;//resalta componentes verticales
        imshow("SIN crecimiento",frameGray);
        cv::Mat FrameSeed;
        FrameSeed =gSetMaskSeed2(frameGray,img_sub,0,50,255,20);

        imshow("COn crecimiento",FrameSeed);
        cv::waitKey(0);

        }

    }

}

void lluvia::on_pushButton_released()
{
    //ABRE VENTANA CARGA ARCHIVO
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open Video"), ".",
    tr("Video Files(*.avi *.MP4 *.png *.jpg *.jpeg *.bmp *.tif)"));
    inputName=fileName.toStdString().c_str();
    video.open(inputName);
    play=true;
}

void lluvia::on_pushButton_3_released()
{
    play=false;

}

void lluvia::on_pushButton_2_released()
{
    if(video.isOpened())play=true;
}
