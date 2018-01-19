#ifndef EFFECTARROW_H
#define EFFECTARROW_H

#include "effectbase.h"
#include "parserbase.h"

/**********************************************
********    ��ʾ״̬       |       ����
********----------------------------------------
********    ����          |       �л�
********    ����          |       ��ֹ
********    ������         |       ����
********    ��ʼ          |
**********************************************/

class EffectArrow : public EffectBase
{
    Q_OBJECT
public:
    EffectArrow(QWidget *parent, QRect pRect);

    bool InitEffectRc(STATE_INFO* pEffect);
    bool Start();
    bool analysisEffectParaItem(AREA_ANIMATE* pAreaEffect, QString pLabel, QString pContent);   
    bool PaintEffect(QPainter &p);
public slots:
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
public:
    //>@pState��ʾϣ�������״̬��pOperate��ʾ�ڴ�״̬�������еĲ�����
    void updateEffect(int pState, AREA_OPERATE pOperate);
private:
    int   m_CurRcLabel;
};

#endif // EFFECTARROW_H
