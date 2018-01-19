#include "facedetectform.h"
#include "ui_facedetectform.h"

FaceDetectForm::FaceDetectForm(QWidget *parent) :
    TabPage(parent),
    ui(new Ui::FaceDetectForm)
{
    ui->setupUi(this);

    m_PicTakenCount = 0;
#ifdef SURRPORT_OPENCV
    cam     = 0;
    cascade = 0;
    storage = 0;
#endif

    cascade_name = D_FSRCPATH + D_BOOTDIR + QString("haarcascade_frontalface_alt.xml"); //�������Ҫ�õ��ķ�����

    timer   = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));  // ʱ�䵽����ȡ��ǰ����ͷ��Ϣ

    ui->PictureQueneList->setIconSize(QSize(60, 60));
    ui->PictureQueneList->setViewMode(QListView::ListMode);
    ui->PictureQueneList->setMovement(QListView::Static);
    ui->PictureQueneList->setFlow(QListView::LeftToRight);
    ui->PictureQueneList->setWrapping(true);
}

FaceDetectForm::~FaceDetectForm()
{
    closeCamara();
    delete timer;
    delete ui;
}

/******************************
********* ������ͷ ***********
*******************************/
void FaceDetectForm::openCamara()
{
#ifdef SURRPORT_OPENCV
    cam = cvCreateCameraCapture(0);//������ͷ��������ͷ�л�ȡ��Ƶ
    timer->start(33);              // ��ʼ��ʱ����ʱ�򷢳�timeout()�ź�
#endif
}

/*********************************
********* ��ȡ����ͷ��Ϣ ***********
**********************************/
void FaceDetectForm::readFarme()
{
#ifdef SURRPORT_OPENCV
    frame = cvQueryFrame(cam);      // ������ͷ��ץȡ������ÿһ֡
    // ��ץȡ����֡��ת��ΪQImage��ʽ��QImage::Format_RGB888��ͬ������ͷ�ò�ͬ�ĸ�ʽ��
    QImage image = QImage((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888).rgbSwapped();
    ui->PreviewLabel->setPixmap(QPixmap::fromImage(image));  // ��ͼƬ��ʾ��label��
#endif
}

/*******************************
***�ر�����ͷ���ͷ���Դ�������ͷ�***
********************************/
void FaceDetectForm::closeCamara()
{
#ifdef SURRPORT_OPENCV
    timer->stop();         // ֹͣ��ȡ���ݡ�
    cvReleaseCapture(&cam);//�ͷ��ڴ棻
#endif
}

/*************************
********* ���� ***********
**************************/
void FaceDetectForm::takingPictures()
{
#ifdef SURRPORT_OPENCV
    frame = cvQueryFrame(cam);// ������ͷ��ץȡ������ÿһ֡
    //��ץȡ����֡��ת��ΪQImage��ʽ��QImage::Format_RGB888��ͬ������ͷ�ò�ͬ�ĸ�ʽ��
    QImage image = QImage((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888).rgbSwapped();
    if(addPictureItem(image, D_GrabScreenPath + QString("%1.bmp").arg(m_PicTakenCount)))
        m_PicTakenCount++;
#endif
}

bool FaceDetectForm::addPictureItem(QImage pImage, QString pName)
{
    pImage.save(pName);
    IDE_TRACE_STR(pName);
    QListWidgetItem *tmpItem = new QListWidgetItem(QIcon(QPixmap::fromImage(pImage)), getFileName(pName));
    tmpItem->setStatusTip(pName);
    ui->PictureQueneList->addItem(tmpItem);
    return true;
}

void FaceDetectForm::on_PictureQueneList_itemDoubleClicked(QListWidgetItem *item)
{
    QString tmpPixName = item->statusTip();
#ifdef SURRPORT_OPENCV
    DetectOnePixmap(tmpPixName);
#endif
}

#ifdef SURRPORT_OPENCV
void FaceDetectForm::detect_and_draw(IplImage *img)
{
    static CvScalar colors[] =
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };

    double scale = 1.3;
    IplImage* gray = cvCreateImage(cvSize(img->width,img->height), 8, 1);
    IplImage* small_img = cvCreateImage(cvSize( cvRound (img->width/scale), cvRound(img->height/scale)), 8, 1);

    cvCvtColor( img, gray, CV_BGR2GRAY );
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );
    cvClearMemStorage( storage );

    if( cascade )
    {
        /*����cvHaarDetectObjects���ͼ���е�Ŀ�꣬��OpenCV�ṩ��*/
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage, 1.1, 2, 0 ,
                                            cvSize(30, 30) );
        for( int i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
            CvPoint center;
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
            cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );
        }
    }

    cvShowImage( "result", img );
    //cvResizeWindow( "result", 800, 600 );
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}

void FaceDetectForm::DetectOnePixmap(QString pFile)
{
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name.toLatin1().data(), 0, 0, 0 );   //��������������õķ�����
    if( !cascade )
    {
       IDE_DEBUG(QString("ERROR: Could not load classifier cascade"));
       return ;
    }
    storage = cvCreateMemStorage(0);                                       //��̬�洢�ṹ�������洢������ͼ���е�λ��
    cvNamedWindow( "result", 1 );
    const char* filename = pFile.toLatin1().data();//�����ͼ��(��������·��)
    IDE_TRACE_STR(pFile);
    IplImage* image = cvLoadImage( filename, 1 );                          //����ͼ��
    if(image)
    {
        detect_and_draw( image );                                              //�Լ��ص�ͼ����м��
        cvWaitKey(0);
        cvReleaseImage( &image );
        cvDestroyWindow("result");
    }
}
#endif

void FaceDetectForm::on_BtnOpenCamera_clicked()
{
    QString tmpString = ui->BtnOpenCamera->text();
    if(tmpString.compare("OpenCamera",Qt::CaseInsensitive) == 0)
    {
        ui->BtnOpenCamera->setText("CloseCamera");
        ui->BtnOpenCamera->setIcon(QIcon(":/Menu/rc/Menu/pass.png"));
        openCamara();
    }
    else if(tmpString.compare("CloseCamera",Qt::CaseInsensitive) == 0)
    {
        ui->BtnOpenCamera->setText("OpenCamera");
        ui->BtnOpenCamera->setIcon(QIcon(":/Menu/rc/Menu/notpass.png"));
        closeCamara();
    }
}

void FaceDetectForm::on_BtnTakePicture_clicked()
{
    takingPictures();
}
