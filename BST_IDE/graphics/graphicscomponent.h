#ifndef D_GraphicsComponent_H
#define D_GraphicsComponent_H

#include "graphicsrc.h"

#define D_SETCTRL_ZVALUE  10000 //>@���ÿؼ�Z��
#define D_MSGCTRL_ZVALUE  10001 //>@��Ϣ�ؼ�Z��
#define D_PRGCTRL_ZVALUE  10002 //>@�������ؼ�Z��
#define D_KEYBOARD_ZVALUE  10003//>@���̿ؼ�Z��

class GraphicsComponent : public QGraphicsWidget, public GraphicsRC
{
    Q_OBJECT
public:
#ifdef  SURRPORT_3D
    friend class GraphicsGLWidget;
#endif
    GraphicsComponent(QGraphicsItem *parent, GraphicsScene *pScene, QRectF pRect);
    ~GraphicsComponent();

    void ReleaseTextures();

    QRectF boundingRect() const;
    QPainterPath shape() const;

    void SetComGeometory(QRectF pRect);

#ifdef  SURRPORT_3D
    void setGeometry3D(qreal x, qreal y, qreal width, qreal height);
    void setGeometry3D(QRectF rect);
    void BindTexture(GLuint L_Texture,GLfloat R,GLfloat G,GLfloat B);
    void UnbindTexture();
#endif

    virtual bool Start();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual bool PaintEffect(QPainter *p);

#ifdef  SURRPORT_3D
    void OpenGL_PaintBegin(QPainter *painter);
    void OpenGL_PaintEnd(QPainter *painter);
    void OpenGL_Effect(QPainter *painter);
#endif

    void Play();
    void Stop();
    bool SetEnvironment(QString pEnvirString);  //>@����COM���ֳ�����
    bool GetEnvironment(QString &pEnvirString); //>@��ȡCOM���ֳ�����

    bool SetPara(QDomElement &pElement, xmlParse *pParse); //>@�޸�pElementΪ���Element
    bool GetPara(QDomElement &pElement, xmlParse *pParse);

    bool SetElementText(QDomElement pElement, QString pValue);
    void UpdateProgress(QString pMsg, int pValue);

    //>@��ȡ���ڵ��е���Ҫ��Ϣ
    virtual QString GetInformation() { return QString("%1").arg(m_ComElement.tagName()); }

    //>@�κζ������ֻ��Ҫ��������������������һ���϶���״̬CHANGE���ڶ���һ����״̬�Ĳ����仯
    void OperateStep0();

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

public:
    void PressEvent(QGraphicsSceneMouseEvent *event);
    void MoveEvent(QGraphicsSceneMouseEvent *event);
    void ReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void MouseRelease(QPointF Pos);
    void reportEffect(AREA_OPERATE pOperate, QVariant pPara);

public slots:
    virtual void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    virtual void UpdateCom(int pFrame);
    virtual void ExecAction(QAction *pAction) {}
    void OperateStep1();

public:
    QTimeLine                           m_Animator;
    AREA_ANIMATE                       *m_AreaAnimate;
    OPERATE_INFO                        m_OperateInfo[2];
    PIXMAP_POINTER                      m_PixmapPointer;

    //>@����UpdateEffect��
    AREA_OPERATE                        m_UpdateOPerate;
    QVariant                            m_UpdatePara;

    bool                                m_PaintBusy;
    bool                                m_PaintOptimizer; //>@�Ż����Ʋ���

    QActionGroup*                       m_ActionGroup;

public:
#ifdef  SURRPORT_3D
    GraphicsGLWidget                   *m_GLWidget;
    int                                 m_3DWidth, m_3DHeight;
    //>@����XYZ������ת�Լ���ȵı仯
    int                                 m_3DXDiff,m_3DYDiff,m_3DZDiff; //>@���������ϵĽǶ�ƫ��
    qreal                               m_3DXScale,m_3DYScale,m_3DZScale; //>@���������ϵ�����
    float                               m_3DDeepDiff;
    GLfloat                             m_3DVerPoint[7200];
    GLfloat                             m_3DVerTexture[7200];
    GLfloat                             m_3DNormalFront[7200];
#endif

public:
    void StartTimeline();
    void StopAnimate();

    QPixmap GetFlipPixmap(QPixmap pPixmap, QRectF pTargetRect);
    bool DrawPixmap(QPainter *p, QPixmap pPixmap, QSizeF pSize, QRectF pTargetRect, QRectF pSourceRect);
    bool DrawPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect, QRectF pSourceRect);
    bool DrawFlipPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect);
    bool DrawPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect);
    bool DrawPixmap(QPainter *p, QPixmap pPixmap, QRect pTargetRect) {  return DrawPixmap(p, pPixmap, QRectF(pTargetRect)); }
    bool DrawPixmap(QPainter *p, QPixmap pPixmap);

    void Paint2D_None(QPainter *p, QRectF targetRect);
    void Paint2D_Flip(QPainter *p,QRectF targetRect);
    void Paint2D_Roll(QPainter *p,QRectF targetRect);
    void Paint2D_Zoom(QPainter *p,QRectF targetRect);
    void Paint2D_Rotate(QPainter *p,QRectF targetRect);
    void Paint2D_Twirl(QPainter *p, QRectF targetRect);
    void Paint2D_Blink(QPainter *p,QRectF targetRect);
    void Paint2D_Slipcycle(QPainter *p,QRectF targetRect);
    void Paint2D_Fireworks(QPainter *p, QRectF targetRect);
    void Paint2D_Transparent(QPainter *p,QRectF targetRect);
    void Paint2D_Spread(QPainter *p,QRectF targetRect);
    void Paint2D_Wheel(QPainter *p,QRectF targetRect);

#ifdef  SURRPORT_3D
    void Paint3D_CircleSurface(GLuint texture);//>@����Բ��
    void Paint3D_CamberSurface(GLuint texture);//>@���ƻ���
    void Paint3D_Cube(QPainter *p,QRectF targetRect);    //>@����������
    void Paint3D_Cylinder(QPainter *p,QRectF targetRect);//>@����Բ����
    void Set3DEffect(EffectType pType);
#endif
};

#ifdef  SURRPORT_3D

class GraphicsGLWidget : public QGLWidget
{
public:
    explicit GraphicsGLWidget() : QGLWidget()
    {
    }
    ~GraphicsGLWidget()
    {
    }
    bool InitTexture(GraphicsComponent *pGraphicsWidget)
    {
        if(pGraphicsWidget = 0)
            return false;
        QList<int> tmpKeyList = pGraphicsWidget->m_EffectPath.keys();
        int count = tmpKeyList.count();
        int tmpKey;
        for(int i=0;i<count;i++)
        {
            tmpKey = tmpKeyList.at(i);
            QPixmap tmpPixmap;
            tmpPixmap.load(pGraphicsWidget->m_EffectPath.value(tmpKey));
            if(tmpPixmap.isNull())
                continue;
            GLuint tmpGLuint = bindTexture(tmpPixmap);
            pGraphicsWidget->m_EffectTexture.insert(tmpKey, tmpGLuint);
        }
        return true;
    }
    GLuint GetTexture(QString pImagePath, QSizeF pSize)
    {
        QImage tmpImage(pImagePath);
        if(tmpImage.isNull())
        {
            IDE_TRACE_STR(pImagePath);
            return 0;
        }
        return bindTexture(QPixmap::fromImage(tmpImage.scaled(pSize.toSize())));
    }
};

#endif

inline void GraphicsComponent::StartTimeline()
{
    m_Animator.stop();
    m_Animator.setFrameRange(m_AreaAnimate->mFrameStart, m_AreaAnimate->mFrameEnd);
    m_AreaAnimate->mFrameCount = m_AreaAnimate->mFrameEnd - m_AreaAnimate->mFrameStart - 1;
    m_Animator.setDuration(m_AreaAnimate->mAnimateTime);
    m_Animator.setCurveShape(m_AreaAnimate->mAnimateType);
    m_Animator.setLoopCount(m_AreaAnimate->mLoopCount);
    m_Animator.start();
}

inline bool GraphicsComponent::DrawPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect, QRectF pSourceRect)
{
    if(pTargetRect.isValid() == false || pSourceRect.isValid() == false)
    {
        //IDE_TRACE();
        return false;
    }
    if(pPixmap.isNull() == false)
    {
        p->drawPixmap(pTargetRect,
                     pPixmap,
                     pSourceRect);
        return true;
    }
    return false;
}

inline bool GraphicsComponent::DrawPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect)
{
    if(pTargetRect.isValid() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(pPixmap.isNull() == false)
    {
        p->drawPixmap(pTargetRect.toRect(), pPixmap);
        return true;
    }
    return false;
}

//>@ֻ����������ͼƬ�ķ�ת��ʾ
inline bool GraphicsComponent::DrawFlipPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect)
{
    if(pTargetRect.isValid() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(pPixmap.isNull() == false)
    {
        p->save();
        p->translate(pTargetRect.width()/2.0, pTargetRect.height()/2.0);
        p->setTransform((QTransform().translate(pTargetRect.width()/2.0, pTargetRect.height()/2.0)
                        .rotate(180, Qt::XAxis)
                        .translate(-pTargetRect.width()/2.0, -pTargetRect.height()/2.0)));
        p->drawPixmap(pTargetRect.toRect(), pPixmap);
        p->restore();
        return true;
    }
    return false;
}

inline QPixmap GraphicsComponent::GetFlipPixmap(QPixmap pPixmap, QRectF pTargetRect)
{
    if(pPixmap.isNull())
    {
        IDE_TRACE();
        return QPixmap();
    }
    QImage tmpImage(pTargetRect.width(), pTargetRect.height(), QImage::Format_ARGB32_Premultiplied);
    if(tmpImage.isNull())
        return QPixmap();
    tmpImage.fill(Qt::transparent); //>@����͸��ͼ��
    QPainter p(&tmpImage);
    p.translate(pTargetRect.width()/2.0, pTargetRect.height()/2.0);
    p.setTransform((QTransform().translate(pTargetRect.width()/2.0, pTargetRect.height()/2.0)
                    .rotate(180, Qt::XAxis)
                    .translate(-pTargetRect.width()/2.0, -pTargetRect.height()/2.0)));
    DrawPixmap(&p, pPixmap, pTargetRect);
    p.end();
    return QPixmap::fromImage(tmpImage);
}

inline bool GraphicsComponent::DrawPixmap(QPainter *p, QPixmap pPixmap)
{
    if(pPixmap.isNull() == false)
    {
        p->drawPixmap(0, 0, pPixmap);
        return true;
    }
    return false;
}

inline bool GraphicsComponent::DrawPixmap(QPainter *p, QPixmap pPixmap, QSizeF pSize, QRectF pTargetRect, QRectF pSourceRect)
{
    if(pTargetRect.isValid() == false || pSourceRect.isValid() == false || pSize.isValid() == false)
    {
        return false;
    }
    if(pPixmap.isNull() == false)
    {
        p->drawPixmap(pTargetRect,
                      pPixmap.scaled(pSize.toSize(),Qt::IgnoreAspectRatio),
                      pSourceRect);
        return true;
    }
    return false;
}

//>@��Ҫ���Ƶ�ͼ����ЧͼƬ
inline void GraphicsComponent::Paint2D_Roll(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    qreal tmpDiff;
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal index = m_Animator.currentFrame();
    qreal x, y;
    //>@���Ƚ�����ԭ���Ƶ���ǰ��Ҫ���Ƶ�����
    if(m_AreaAnimate->mDirection == Vertical)
    {
        p->save();
        tmpDiff = hh / m_AreaAnimate->mFrameCount;
        if(m_AreaAnimate->mOrientation == Positive)
        {
            y = (qreal)(tmpDiff*index);
            p->translate(targetRect.left(), targetRect.top()+y);
            DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
        }
        else
        {
            y = (qreal)(tmpDiff*index);
            p->translate(targetRect.left(), targetRect.top()-y);
            DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
        }
        p->restore();
        p->save();
        if(m_AreaAnimate->mOrientation == Positive)
        {
            y = hh - y;
            p->translate(targetRect.left(), targetRect.top()-y);
            DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
        }
        else
        {
            y = hh - y;
            p->translate(targetRect.left(), targetRect.top()+y);
            DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
        }
        p->restore();
    }
    else
    {
        p->save();
        tmpDiff = (qreal)(hw) / m_AreaAnimate->mFrameCount;
        if(m_AreaAnimate->mOrientation == Positive)
        {
            x = (qreal)(tmpDiff*index);
            p->translate(targetRect.left()+x, targetRect.top());
            DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(0,0,hw-x,hh), QRectF(0,0,hw-x,hh));
        }
        else
        {
            x = (qreal)(tmpDiff*index);
            p->translate(targetRect.left()-x, targetRect.top());
            DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(x,0,hw-x,hh), QRectF(x,0,hw-x,hh));
        }
        p->restore();
        p->save();
        if(m_AreaAnimate->mOrientation == Positive)
        {
            x = hw - x;
            p->translate(targetRect.left()-x, targetRect.top());
            DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(x,0,hw-x,hh), QRectF(x,0,hw-x,hh));
        }
        else
        {
            x = hw - x;
            p->translate(targetRect.left()+x, targetRect.top());
            DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(0,0,hw-x,hh), QRectF(0,0,hw-x,hh));
        }
        p->restore();
    }
}

inline void GraphicsComponent::Paint2D_None(QPainter *p, QRectF targetRect)
{
    DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect);
}

//>@��Ҫ���Ƶ�ͼ����ЧͼƬ
inline void GraphicsComponent::Paint2D_Flip(QPainter *p, QRectF targetRect)
{
    qreal w = targetRect.width();
    qreal h = targetRect.height();
    qreal hw = w / 2.0;
    qreal hh = h / 2.0;
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    // behind is the new pixmap
    DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect);
    if(m_AreaAnimate->mFrameCount <= 0)
        return;
    qreal halfcount =  m_AreaAnimate->mFrameCount / 2.0;
    qreal index = m_Animator.currentFrame();

    p->save();
    if (index <= halfcount)
    {
        // the top part of the old pixmap is flipping
        qreal angle;
        if(m_AreaAnimate->mOrientation == Positive)
        {
            angle = -180.0 * index / m_AreaAnimate->mFrameCount;
        }
        else
        {
            angle = 180.0 * index / m_AreaAnimate->mFrameCount;
        }
        p->translate(targetRect.left() + hw, targetRect.top() + hh);
        if(m_AreaAnimate->mDirection == Horizontal)
            p->setTransform(QTransform().rotate(angle, Qt::YAxis), true);
        else
            p->setTransform(QTransform().rotate(angle, Qt::XAxis), true);
        DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(-hw, -hh, w, h), QRectF(0,0,w,h));

        // the bottom part is still the old pixmap
        p->restore();
        p->save();
        p->translate(targetRect.left(), targetRect.top());
        if(m_AreaAnimate->mDirection == Horizontal)
            if(m_AreaAnimate->mOrientation == Positive)
                p->setClipRect(hw, 0, hw, targetRect.height());
            else
                p->setClipRect(0, 0, hw, targetRect.height());
        else
            if(m_AreaAnimate->mOrientation == Positive)
                p->setClipRect(0, hh, targetRect.width(), hh);
            else
                p->setClipRect(0, 0, targetRect.width(), hh);
        DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(0,0,w,h), QRectF(0,0,w,h));
    }
    else
    {
        p->translate(targetRect.left(), targetRect.top());
        if(m_AreaAnimate->mDirection == Horizontal)
            if(m_AreaAnimate->mOrientation == Positive)
                p->setClipRect(hw, 0, hw, targetRect.height());
            else
                p->setClipRect(0, 0, hw, targetRect.height());
        else
            if(m_AreaAnimate->mOrientation == Positive)
                p->setClipRect(0, hh, targetRect.width(), hh);
            else
                p->setClipRect(0, 0, targetRect.width(), hh);

        // the bottom part is still the old pixmap
        DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(0,0,w,h), QRectF(0,0,w,h));
        // the bottom part of the new pixmap is flipping
        qreal angle;
        if(m_AreaAnimate->mOrientation == Positive)
        {
            angle = 180.0 - 180.0 * index / m_AreaAnimate->mFrameCount;
        }
        else
        {
            angle = - 180.0 * (m_AreaAnimate->mFrameCount-index) / m_AreaAnimate->mFrameCount;
        }
        p->translate(hw, hh);
        if(m_AreaAnimate->mDirection == Horizontal)
            p->setTransform(QTransform().rotate(angle, Qt::YAxis), true);
        else
            p->setTransform(QTransform().rotate(angle, Qt::XAxis), true);
        DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(-hw, -hh, w, h), QRectF(0,0,w,h));
    }
    p->restore();
}

//>@��Ҫ���Ƶ�ͼ����ЧͼƬ
inline void GraphicsComponent::Paint2D_Zoom(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal halfcount =  m_AreaAnimate->mFrameCount / 2.0;
    qreal index = m_Animator.currentFrame();

    qreal x,y,w,h;
    p->save();
    p->translate(targetRect.left()+hw/2, targetRect.top()+hh/2);
    if(index < halfcount)
    {
        index = halfcount - index;
        x = -hw*index/m_AreaAnimate->mFrameCount;
        y = -hh*index/m_AreaAnimate->mFrameCount;
        w = 2*hw*index/m_AreaAnimate->mFrameCount;
        h = 2*hh*index/m_AreaAnimate->mFrameCount;
        DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(x,y,w,h), QRectF(0,0,hw,hh));
    }
    else
    {
        index = index - halfcount;
        x = -hw*index/m_AreaAnimate->mFrameCount;
        y = -hh*index/m_AreaAnimate->mFrameCount;
        w = 2*hw*index/m_AreaAnimate->mFrameCount;
        h = 2*hh*index/m_AreaAnimate->mFrameCount;
        DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(x,y,w,h), QRectF(0,0,hw,hh));
    }
    p->restore();
}

inline void GraphicsComponent::Paint2D_Blink(QPainter *p,QRectF targetRect)
{
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    p->save();
    int index = m_Animator.currentFrame();
    if(index % 2)
        p->setOpacity(0.2);
    else
        p->setOpacity(1);
    Paint2D_None(p, targetRect);
    p->restore();
}

//>@��ת
inline void GraphicsComponent::Paint2D_Twirl(QPainter *p, QRectF targetRect)
{
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, rect());
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    qreal index = m_Animator.currentFrame();
    p->save();
    p->translate(targetRect.left()+hw/2, targetRect.top()+hh/2);
    qreal angle = 0;
    if(m_AreaAnimate->mOrientation == Positive)
    {
        angle = 360.0 * index / m_AreaAnimate->mFrameCount;
    }
    else if(m_AreaAnimate->mOrientation == Negative)
    {
        angle = - 360.0 * index / m_AreaAnimate->mFrameCount;
    }
    p->rotate(angle);
    p->restore();
}

//>@��Ҫ���Ƶ�ͼ����ЧͼƬ
inline void GraphicsComponent::Paint2D_Rotate(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, rect());
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal halfCount =  m_AreaAnimate->mFrameCount / 2.0;
    qreal index = m_Animator.currentFrame();

    p->save();
    p->translate(targetRect.left()+hw/2.0, targetRect.top()+hh/2.0);
    QPixmap pix = (index <= halfCount) ? m_PixmapPointer.mLastPointer : m_PixmapPointer.mCurPointer;

    qreal angle = (index <= halfCount) ? (-180.0 * index / m_AreaAnimate->mFrameCount) : (180.0 - 180.0 * index / m_AreaAnimate->mFrameCount);
    if(m_AreaAnimate->mDirection == Horizontal)
    {
        p->setTransform(QTransform().rotate(angle, Qt::YAxis), true);
        DrawPixmap(p, pix, QRect(-hw / 2.0, -hh / 2.0, hw, hh));
    }
    else
    {
        p->setTransform(QTransform().rotate(angle, Qt::XAxis), true);
        DrawPixmap(p, pix, QRect(-hw / 2.0, -hh / 2.0, hw, hh));
    }
    p->restore();
}

//>@��Ҫ���Ƶ�ͼ����ЧͼƬ
inline void GraphicsComponent::Paint2D_Slipcycle(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
        return;
    qreal halfCount =  m_AreaAnimate->mFrameCount / 2.0;
    if(halfCount <= 0)
        return;
    qreal index = m_Animator.currentFrame();

    p->save();
    QPixmap tmpPixmap;
    qreal x, y, tmpDiff;
    if(m_AreaAnimate->mDirection == Vertical)
    {
        tmpDiff = (qreal)(hh) / halfCount;
        if(index <= halfCount)
        {
            tmpPixmap = m_PixmapPointer.mLastPointer;
            y = (int)(tmpDiff*index);
            if(y < 0)
                y = 0;
            if(m_AreaAnimate->mOrientation == Positive)
            {
                p->translate(targetRect.left(), targetRect.top()+y);
                //>@���ʹ�����ַ��������ˢ����ť��Χ��������Ȼ��һ���ȽϺÿ�����Ч��
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
            }
            else
            {
                p->translate(targetRect.left(), targetRect.top()-y);
                //>@���ʹ�����ַ��������ˢ����ť��Χ��������Ȼ��һ���ȽϺÿ�����Ч��
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
            }
        }
        else
        {
            y = (int)(tmpDiff*(m_AreaAnimate->mFrameEnd - index));
            tmpPixmap = m_PixmapPointer.mCurPointer;
            if(y < 0)
                y = 0;
            if(m_AreaAnimate->mOrientation == Positive)
            {
                p->translate(targetRect.left(), targetRect.top()+y);
                //>@���ʹ�����ַ��������ˢ����ť��Χ��������Ȼ��һ���ȽϺÿ�����Ч��
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
            }
            else
            {
                p->translate(targetRect.left(), targetRect.top()-y);
                //>@���ʹ�����ַ��������ˢ����ť��Χ��������Ȼ��һ���ȽϺÿ�����Ч��
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
            }
        }
    }
    else
    {
        tmpDiff = (qreal)(hw) / halfCount;
        if(index <= halfCount)
        {
            x = (int)(tmpDiff*index);
            if(x < 0)
                x = 0;
            tmpPixmap = m_PixmapPointer.mLastPointer;
            if(m_AreaAnimate->mOrientation == Positive)
            {
                p->translate(targetRect.left()+x, targetRect.top());
                //>@���ʹ�����ַ��������ˢ����ť��Χ��������Ȼ��һ���ȽϺÿ�����Ч��
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(0,0,hw-x,hh), QRectF(0,0,hw-x,hh));
            }
            else
            {
                p->translate(targetRect.left()-x, targetRect.top());
                //>@���ʹ�����ַ��������ˢ����ť��Χ��������Ȼ��һ���ȽϺÿ�����Ч��
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(x,0,hw-x,hh), QRectF(x,0,hw-x,hh));
            }
        }
        else
        {
            x = (int)(tmpDiff*(m_AreaAnimate->mFrameEnd - index));
            if(x < 0)
                x = 0;
            tmpPixmap = m_PixmapPointer.mCurPointer;
            if(m_AreaAnimate->mOrientation == Positive)
            {
                p->translate(targetRect.left()+x, targetRect.top());
                //>@���ʹ�����ַ��������ˢ����ť��Χ��������Ȼ��һ���ȽϺÿ�����Ч��
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(0,0,hw-x,hh), QRectF(0,0,hw-x,hh));
            }
            else
            {
                p->translate(targetRect.left()-x, targetRect.top());
                //>@���ʹ�����ַ��������ˢ����ť��Χ��������Ȼ��һ���ȽϺÿ�����Ч��
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(x,0,hw-x,hh), QRectF(x,0,hw-x,hh));
            }
        }
    }
    p->restore();
}

inline void GraphicsComponent::Paint2D_Fireworks(QPainter *p, QRectF targetRect)
{

}

//>@��Ҫ���Ƶ�ͼ����ЧͼƬ
inline void GraphicsComponent::Paint2D_Transparent(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal Stage1 = m_AreaAnimate->mFrameCount*3.0/10.0;
    qreal Stage2 = m_AreaAnimate->mFrameCount*4.0/10.0;
    //qreal Stage3 = (qreal)(m_AreaAnimate->mFrameCount)*3.0/10.0;

    qreal index = m_Animator.currentFrame();
    //>@ѭ���ı�͸����ͼƬ
    qreal tmpLastAlpha, tmpAlpha;
    if(index <= Stage1)
    {
        tmpLastAlpha = (qreal)(Stage1 - index) / (qreal)Stage1;;
        tmpAlpha = (qreal)(index) / (qreal)Stage1;
    }
    else if(index <= Stage2 + Stage1)
    {
        tmpLastAlpha = 0;
        tmpAlpha = 1;
    }
    else  //>@�������ӣ�����һ�ζ�����ɺ�Ҳ��index>Stage2 + Stage1ʱ�Ὣ����ͼƬ������͸������
    {
        tmpLastAlpha = 0;
        tmpAlpha = 1;
    }

    p->save();
    p->translate(targetRect.left(), targetRect.top());
    p->setOpacity(tmpLastAlpha);
    DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(0,0,hw,hh), QRectF(0,0,hw,hh));
    p->setOpacity(tmpAlpha);
    DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(0,0,hw,hh), QRectF(0,0,hw,hh));
    p->restore();
}

//>@ֻ��Ҫ������ЧͼƬ����
inline void GraphicsComponent::Paint2D_Spread(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    qreal x,y,h,w;
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
        return;
    qreal halfCount =  m_AreaAnimate->mFrameCount / 2.0;
    qreal index = m_Animator.currentFrame();

    if(index < halfCount)
    {
        index = halfCount - index;
        x = -hw*index/m_AreaAnimate->mFrameCount;
        y = -hh*index/m_AreaAnimate->mFrameCount;
        w = 2*hw*index/m_AreaAnimate->mFrameCount;
        h = 2*hh*index/m_AreaAnimate->mFrameCount;
    }
    else
    {
        index = index - halfCount;
        x = -hw*index/m_AreaAnimate->mFrameCount;
        y = -hh*index/m_AreaAnimate->mFrameCount;
        w = 2*hw*index/m_AreaAnimate->mFrameCount;
        h = 2*hh*index/m_AreaAnimate->mFrameCount;
    }
    p->save();
    p->translate(targetRect.left()+hw/2, targetRect.top()+hh/2);
    DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(x,y,w,h), QRectF(0,0,hw,hh));
    p->restore();
}

//>@ֻ��Ҫ������ЧͼƬ����
inline void GraphicsComponent::Paint2D_Wheel(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
        return;
    qreal index = m_Animator.currentFrame();

    qreal tmpDiff = (qreal)(2.0 * PI) / (qreal)(m_AreaAnimate->mFrameCount) * 2.0;
    qreal rotate = 360.0 * normalizeAngle(tmpDiff*index) / 2.0 /PI;

    p->save();
    p->translate(targetRect.left()+hw/2, targetRect.top()+hh/2);
    p->rotate(rotate);
    DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(-hw/2.0, -hh/2.0, hw, hh), QRectF(0,0,hw,hh));
    p->restore();
}

#ifdef  SURRPORT_3D
inline void GraphicsComponent::Paint3D_CircleSurface(GLuint texture)
{
    glPushMatrix();
    glFrontFace(GL_CCW);
    glTranslatef(-0.5f,-0.5f,0.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //�������ǻ�360���㣬����360�������Ρ�
    for(int i=0;i<360;i+=2)
    {
        double aa=(double)PI*i/180;
        m_3DVerPoint[i]=(float)((cos(aa)+1.0)*0.5);   //���������
        m_3DVerPoint[i+1]=(float)((sin(aa)+1.0)*0.5);  //����������
    }
    glVertexPointer(2, GL_FLOAT, 0, m_3DVerPoint);
    BindTexture(texture,1.0f,1.0f,1.0f);
    glTexCoordPointer(2,GL_FLOAT,0,m_3DVerPoint);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 180);
    UnbindTexture();
    glPopMatrix();
}

inline void GraphicsComponent::Paint3D_CamberSurface(GLuint texture)
{
    int fragAngle = 30;
    int n = 180/fragAngle;
    float b = (float)fragAngle/180;
    float r = (float)1/(2*PI)-0.005;
    glPushMatrix();
    glRotatef(-90,0.0f,1.0f,0.0f);
    glScalef(2*PI, 1.0f, 2*PI);
    glTranslatef(0.0f,0.5f,0.0f);
    for(int j=0;j<2;j++)
    {
        for(int i=0;i<n;i++)
        {
            m_3DVerPoint[0] = -0.0;m_3DVerPoint[1] = 0.0;m_3DVerPoint[2] = r;
            m_3DVerPoint[3] = b;m_3DVerPoint[4] = -1.0;m_3DVerPoint[5] = r;
            m_3DVerPoint[6] = -0.0;m_3DVerPoint[7] = -1.0;m_3DVerPoint[8] = r;

            m_3DVerPoint[9] = b;m_3DVerPoint[10] = -1.0;m_3DVerPoint[11] = r;
            m_3DVerPoint[12] = -0.0;m_3DVerPoint[13] = 0.0;m_3DVerPoint[14] = r;
            m_3DVerPoint[15] = b;m_3DVerPoint[16] = 0.0;m_3DVerPoint[17] = r;
            GLfloat NormalFront[] = {
                                        0,0,-1, 0,0,-1, 0,0,-1,
                                        0,0,-1, 0,0,-1, 0,0,-1
                                    };
            memcpy(m_3DVerTexture,m_3DVerPoint,18*sizeof(GLfloat));
            m_3DVerTexture[0] = m_3DVerPoint[0]+b*i;
            m_3DVerTexture[3] = m_3DVerPoint[3]+b*i;
            m_3DVerTexture[6] = m_3DVerPoint[6]+b*i;

            m_3DVerTexture[9] = m_3DVerPoint[9]+b*i;
            m_3DVerTexture[12] = m_3DVerPoint[12]+b*i;
            m_3DVerTexture[15] = m_3DVerPoint[15]+b*i;

            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3,GL_FLOAT,0,m_3DVerPoint);
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT,0,NormalFront);
            BindTexture(texture,1.0f,1.0f,1.0f);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(3,GL_FLOAT,0,m_3DVerTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glRotatef(fragAngle, 0, 1, 0);
        }
    }
    UnbindTexture();
    glPopMatrix();
}

inline void GraphicsComponent::Paint3D_Cube(QPainter *p,QRectF targetRect)
{
    OpenGL_PaintBegin(p);
    OpenGL_Effect(p);

    glPushMatrix();
    //glRotatef(180,0.0f,0.0f,1.0f);
    glPushMatrix();
    glFrontFace(GL_CCW);
    GLfloat afVerticesFront[] = {
                                    -0.5f,  0.5f,   0.5f,
                                    0.5f,   0.5f,   0.5f,
                                    -0.5f,  -0.5f,  0.5f,
                                    0.5f,   -0.5f,  0.5f,
                                };
    GLfloat afNormalsFront[] = {
                                    0,0,1, 0,0,1, 0,0,1,
                                    0,0,1, 0,0,1, 0,0,1
                                };
    GLfloat afTexCoordFront[] = {
                                    0.0f, 1.0f,
                                    1.0f, 1.0f,
                                    0.0f, 0.0f,
                                    1.0f, 0.0f
                                };
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //>@Front
    glVertexPointer(3,GL_FLOAT,0,afVerticesFront);
    glNormalPointer(GL_FLOAT,0,afNormalsFront);
    BindTexture(m_GLWidget->GetTexture(m_EffectPath.value(0), targetRect.size()),1.0f,1.0f,1.0f); //CubeFront
    glTexCoordPointer(2,GL_FLOAT,0,afTexCoordFront);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //>@Top
    glPopMatrix();
    BindTexture(m_GLWidget->GetTexture(m_EffectPath.value(1), targetRect.size()),1.0f,1.0f,1.0f);//CubeTop
    glRotatef(90,1.0f,0.0f,0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //>@Back
    glPopMatrix();
    BindTexture(m_GLWidget->GetTexture(m_EffectPath.value(2), targetRect.size()),1.0f,1.0f,1.0f);//CubeBack
    glRotatef(180,1.0f,0.0f,0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //>@Bottom
    glPopMatrix();
    BindTexture(m_GLWidget->GetTexture(m_EffectPath.value(3), targetRect.size()),1.0f,1.0f,1.0f);//CubeBottom
    glRotatef(270,1.0f,0.0f,0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //>@Right
    glPopMatrix();
    BindTexture(m_GLWidget->GetTexture(m_EffectPath.value(4), targetRect.size()),1.0f,1.0f,1.0f);//CubeRight
    glRotatef(90,0.0f,1.0f,0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //>@Left
    glPopMatrix();
    BindTexture(m_GLWidget->GetTexture(m_EffectPath.value(5), targetRect.size()),1.0f,1.0f,1.0f);//CubeLeft
    glRotatef(270,0.0f,1.0f,0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    UnbindTexture();
    glPopMatrix();
    glPopMatrix();

    OpenGL_PaintEnd(p);
}

inline void GraphicsComponent::Paint3D_Cylinder(QPainter *p,QRectF targetRect)
{
    OpenGL_PaintBegin(p);
    OpenGL_Effect(p);

    glPushMatrix();
    glScalef(0.5, 0.2, 0.5);
    glPushMatrix();
    Paint3D_CamberSurface(m_GLWidget->GetTexture(m_EffectPath.value(0), targetRect.size()));//SideSurface
    glTranslatef(0.0f,0.5f,0.0f);
    glRotatef(90,1.0f,0.0f,0.0f);
    glRotatef(180,0.0f,0.0f,1.0f);
    glScalef(1.94, 1.94, 1);
    Paint3D_CircleSurface(m_GLWidget->GetTexture(m_EffectPath.value(1), targetRect.size()));//TopCircleSurface
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f,-0.5f,0.0f);
    glRotatef(-90,1.0f,0.0f,0.0f);
    glScalef(1.94, 1.94, 1);
    Paint3D_CircleSurface(m_GLWidget->GetTexture(m_EffectPath.value(2), targetRect.size()));//BottomCircleSurface
    glPopMatrix();
    glPopMatrix();

    OpenGL_PaintEnd(p);
}
#endif

#endif // GraphicsComponent_H
