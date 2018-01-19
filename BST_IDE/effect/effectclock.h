#ifndef EFFECTCLOCK_H
#define EFFECTCLOCK_H

#include "effectbase.h"

class EffectClock : public EffectBase
{
    Q_OBJECT
public:
    EffectClock(QWidget *parent, QRect pRect);

    bool InitEffectRc(STATE_INFO* pEffect);
    void CalcElementInfo(CLOCK_TYPE pType);
    void CalcAnalogClockPointer();
    bool Start();
    bool analysisEffectParaItem(AREA_ANIMATE* pAreaEffect, QString pLabel, QString pContent);
    bool PaintEffect(QPainter &p);
public slots:
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
public:
    void updateEffect(CLOCK_INFO pTime);
private:
    CLOCK_LAYOUT             m_Layout;
    CLOCK_ALIGN              m_ClockAlign;
    CLOCK_TYPE               m_Type;

    //>@Digital : ͼԪ(ָ��ʾ���ֵ�һ��������)����Ϊ19����RectYear0~RectSecond1 ---- 0~9 SUN~WES : -
    //>@          ͼƬ��Դ����Ϊ10+7+1+1 = 19
    //>@          0~9 Sun~Sat : -
    //>@Analog : ͼԪ����Ϊ11+3����RectYear0~RectWeek  ʱָ�� ��ָ�� ��ָ��
    //>@         ͼƬ��Դ����Ϊ10+7+1+4+1 = 23(1������������ĵ�)��
    //>@         0~9 Sun~Sat - ʱָ�� ��ָ�� ��ָ�� ʱ�ӵ�ͼ
    QHash<CLOCKE_ELEMENT, CLOCK_ELEMENTINFO*>    m_ClockElement;
    qreal                    m_Diff;                 //>@ͼƬһ���ƶ�(��ת)����
    CLOCK_INFO               m_SavedTime;
    qreal                    m_ItemWidth, m_ItemHeight;
    int                      m_FontSize;

    //>@���Analogʱ��
    qreal       m_PointerLength[3];
    qreal       m_PointerAngle[3][2];   //>@����ָ�룬����ָ���ȺͽǶ�(����ת�ĽǶȣ�Ҳ���ǶȲ�)
};

#endif // EFFECTCLOCK_H
