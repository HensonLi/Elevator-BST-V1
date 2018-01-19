#ifndef GraphicsKEYBOARD_H
#define GraphicsKEYBOARD_H

#include "graphicsbase.h"

class GraphicsKey : public GraphicsBase
{
    Q_OBJECT
public:
    GraphicsKey(QGraphicsItem *parent, QRectF pRect);

    void SetTypeList(QList<quint32> pType)
    {
        m_KeyTypeList = pType;
        if(m_KeyTypeList.count() > 0)
            m_CurTypeIndex = 0;
        else
            m_CurTypeIndex = -1;
    }
    void SetTypeList(quint32 pType)
    {
        m_KeyTypeList.clear();
        m_KeyTypeList.append(pType);
        m_CurTypeIndex = 0;
    }
    quint32 GetCurType()
    {
        if(m_CurTypeIndex < 0)
            return 0;
        return m_KeyTypeList.at(m_CurTypeIndex);
    }

    bool InitEffectRc(STATE_INFO* pEffect);
    bool Start();
    bool analysisEffectParaItem(AREA_ANIMATE* pAreaEffect, QString pLabel, QString pContent);
    virtual bool PaintEffect(QPainter *p);
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void UpdateCom(int pFrame) { GraphicsBase::UpdateCom(pFrame); }
    void SetAlternateType(int index)
    {
        if(index < 0 || index >= m_KeyTypeList.count())
            m_CurTypeIndex = -1;
        else
            m_CurTypeIndex = index;
    }
    void SetAlternateType()
    {
        if(m_CurTypeIndex < m_KeyTypeList.count()-1)
            m_CurTypeIndex ++;
        else
            m_CurTypeIndex = 0;
    }

public:
    BUTTON_RC           m_KeyState;
private:
    QList<quint32>      m_KeyTypeList;
    int                 m_CurTypeIndex;
};

#define MAX_DIALCOUNT  3 //>@dial�������������ַ���

class GraphicsLCD : public GraphicsBase
{
    Q_OBJECT
public:
    GraphicsLCD(QGraphicsItem *parent, QRectF pRect);

    bool InitEffectRc(STATE_INFO* pEffect);
    bool Start();
    bool analysisEffectParaItem(AREA_ANIMATE* pAreaEffect, QString pLabel, QString pContent);
    virtual bool PaintEffect(QPainter *p);

    void SetPattern(KB_PATTERN pPattern) {m_Pattern = pPattern;}

    QPixmap GetBgImage();
    QPixmap GetFloorPixmap();
    QPixmap GetDialPixmap();
    QPixmap GetIconPixmap();
    quint32 GetMappedFloor(quint32 pType);
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void UpdateCom(int pFrame) { GraphicsBase::UpdateCom(pFrame); }
private:
    KB_PATTERN          m_Pattern;
    //>@¥����ʾ����
    QRectF              m_FloorRect;
    char               *m_FloorAsciiList;
    int                 m_FloorAsciiMax, m_FloorAsciiIndex;
    QFont               m_FloorFont;
    //>@���Բ���
    QRectF              m_DialNumRect;
    char                m_DialNumList[MAX_DIALCOUNT+1];
    int                 m_DialNumIndex;
    QFont               m_DialNumFont;
    //>@����ͼ����ʾ����
    bool                m_IconRectValid;
    QRectF              m_IconRect;
};

class GraphicsKeyboard : public GraphicsBase
{
    Q_OBJECT
public:
    typedef struct{
        QString  mLabel;
        qreal    mWidthRatio;  //>@���ַ��Ŀ�Ȼ��߸߶ȱ���
        qreal    mHeightRatio;  //>@���ַ��Ŀ�Ȼ��߸߶ȱ���
    } KEYINFO;
public:
    class KeyboardBtnInfo{
    public:
        KeyboardBtnInfo()
        {
            mButtonItem = 0;
        }
        ~KeyboardBtnInfo()
        {
            if(mButtonItem)
            {
                if(mButtonItem->scene())
                    mButtonItem->scene()->removeItem(mButtonItem);
                mButtonItem->deleteLater();
            }
        }
    public:
        GraphicsKey             *mButtonItem;
        QRectF                      mRect;
    };
public:
    GraphicsKeyboard(QGraphicsItem *parent, QRectF pRect);
    ~GraphicsKeyboard()
    {
        ReleaseKeyboardBtnInfoList();
    }
    bool InitEffectRc(STATE_INFO* pEffect);
    bool Start();
    bool analysisEffectParaItem(AREA_ANIMATE* pAreaEffect, QString pLabel, QString pContent);
    virtual bool PaintEffect(QPainter *p) { return true; }

    bool InitKeyboardBtnInfoList();
    void ReleaseKeyboardBtnInfoList();

    bool AddLCD(QRectF pRect);
    bool AddButton(QString pKeys, GraphicsKey *&pButton, QRectF pRect);

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

signals:
    void sAlternateType();

public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void collectReport(AREA_OPERATE pOperate, QVariant pPara);
    void HideLCD();

public:
    QList<KeyboardBtnInfo*>    m_KeyboardBtnInfoList;  //>@
    GraphicsLCD               *m_LCD;
    KB_PATTERN                 m_Pattern;
    int                        mRSpace, mCSpace;

    QTimer                     m_LCDHideTimer;
};

class DesignKeyboard : public GraphicsKeyboard
{
    Q_OBJECT
public:
    DesignKeyboard(QGraphicsItem *parent, QRectF pRect);

    QSize GetRcSize(QString pRcName);
    QList<QAction*> GetActionList() { QList<QAction*> tmpList; return tmpList; }

    bool PaintEffect(QPainter *p)
    {
        GraphicsKeyboard::PaintEffect(p);
        //>@���Ʊ߿�
        QRectF tmpRect = geometry();
        qreal Width = tmpRect.width();
        qreal Height = tmpRect.height();
        p->translate(Width / 2, Height / 2);
        p->setPen(Qt::DashDotDotLine);
        p->setPen(QColor(10,10,255));
        p->drawRect(QRect(-Width/2, -Height/2, Width, Height));
        p->drawRect(QRect(-Width/2+1, -Height/2+1, Width-2, Height-2));
        return true;
    }

public slots:
    void ExecAction(QAction *pAction) {}
};

#endif // EFFECTKEYBOARD_H
