#ifndef DESIGNSETBAR_H
#define DESIGNSETBAR_H

#include "designbase.h"

class DesignSetBar : public DesignBase
{
    Q_OBJECT
public:
    DesignSetBar(QGraphicsItem *parent, QRectF pRect);
    bool InitEffectRc(STATE_INFO* pEffect);
    bool analysisEffectParaItem(AREA_ANIMATE* pAreaEffect, QString pLabel, QString pContent);
    bool Start();
    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList();
    void ExecAction(QAction *pAction);
    bool PaintEffect(QPainter *p);
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    //>@���õ�ǰλ����ʾ���ΪpIndex��ITEM
    void updateEffect(int pIndex);
    void updateEffect(MOVE_DIRECTION pDirection);
public:
    bool GetValidRect(QRect &pRect);
    void AddItem(int pIndex, ITEM_TYPE pType, QPixmap pPixmap);
    bool InitItems();
    int GetCurItemIndex();
    SETITEM_INFO* GetCurSetItem();
    bool UpdateItems(int pIndex);
public slots:
    void Operate0();    //>@��תΪ����ģʽ
    void Operate1();    //>@���л���
    void Operate2();    //>@��תΪ����ģʽ
    void OperateItem(int pFrameNum);
public:
    QRect               m_ValidRect;
    QList<SETITEM_INFO*>m_ItemList;
    OPERATE_INFO        m_SlipOperateInfo;
    int                 m_ItemCount;        //>@��ЧITEM�ĸ���
    int                 m_ItemSlipCount;    //>@��ǰ��Ҫ�ƶ����ٸ�Itemλ��
    qreal               m_Diff;             //>@ITEM��ִ����Ч��ֵ��
    qreal               m_CurValue;         //>@ITEM��ǰ����״ֵ̬�����ΪRotate��Ч����Ϊ��ǰ��ת�ĽǶ�
};

class DesignSetBarItem : public QWidget
{
    Q_OBJECT
public:
    DesignSetBarItem(QPixmap pPixmap, QWidget *parent = 0);
    void paintEvent(QPaintEvent *e);
    QPixmap m_Pixmap;
};

#endif // EFFECTSETBAR_H
