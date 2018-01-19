#ifndef EFFECTRENDER_H
#define EFFECTRENDER_H

#include "global.h"

class EffectRender : public QObject
{
    Q_OBJECT
public:
    explicit EffectRender(QObject *parent = 0);

    QRectF viewBoxF() {return mViewBox;}
    void setViewBox(QRectF pViewbox);

    //>@������Ⱦʱ��Ч��ͼƬ
    void SetEffectRc(QString pEffectRc);

    bool InitEffectPara(QString pEffectPara);
    virtual bool InitSubEffectPara(QString pLabel, QString pContent);

    void render(QGraphicsItem *pPainter, QRectF pRect);
    void render(QPainter *pPainter);
    void render(QPainter *pPainter, QRectF pRect);

    void StartAnimate();
    void StopAnimate();
    bool isAnimateRunning();  //>@��Ч�Ƿ���������

signals:
    void sRenderFinished();
    
public slots:
    void slot_PaintFrame(int pFrame);
    void slot_EffectState(QTimeLine::State pState);

public:
    QTimeLine               mAnimator;
    QRectF                  mViewBox;

    EffectType              mEffectType;   //>@��Ч���ͣ�Ĭ��Ϊ����Ч
    DIRECTION               mDirection;    //>@ͼƬ��Ч����
    ORIENTATION             mOrientation;  //>@��Ч������
    int                     mLoopCount;    //>@ѭ������
    int                     mFrameStart, mFrameEnd, mFrameCount;
    int                     mAnimateTime;  //>@����ʱ��
    int                     mDutyRatio;
    QTimeLine::CurveShape   mAnimateType;
};

class PixmapRender : public EffectRender
{
    Q_OBJECT
public:
    explicit PixmapRender(QObject *parent = 0);
    ~PixmapRender();

    //>@������Ч������Ⱦ��ͼƬ
    bool load(PIXMAP_POINTER pPixPtr);



public:
    PIXMAP_POINTER          mPixmapPtr;
};

#endif // EFFECTRENDER_H
