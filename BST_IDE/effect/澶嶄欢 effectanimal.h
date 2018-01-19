#ifndef EFFECTANIMAL_H
#define EFFECTANIMAL_H

#include "global.h"

enum _ANIMAL{
    FishTailLeft=0,
    FishTailMiddle,
    FishTailRight,
    InsectStepLeft,
    InsectStepMiddle,
    InsectStepRight
};

typedef enum{
    AnimalFish=0,
    AnimalInsect
}AnimalType;

#define PARA_MOVE 10
#define PARA_ROTATE 11

//>@Ӧ����װ�ε�С����
class EffectAnimal : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit EffectAnimal(QGraphicsItem *parent, QRectF pRect);

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;

    bool InitEffect(COM_STRUCT *pComInfo);
    virtual bool InitEffectRc(STATE_INFO* pEffect);

    virtual bool Start();  //>@ִ����InitEffect��Ҫִ��Start��������Ч

    virtual bool PrepareRc(int pPara);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void PaintEffect(QPainter *painter);

    bool analysisEffectPara(QString pEffectPara);
    virtual bool analysisEffectParaItem(QString pLabel, QString pContent);

public:
    QHash<int, QPixmap *>   m_EffectImage;  //>@ͼƬ��Դ
    AnimalType              m_AnimalType;
    EffectType              m_EffectType;   //>@��Ч���ͣ�Ĭ��Ϊ����Ч
    bool                    m_EffectValid;  //>@��Чʹ��
    QRect                   m_Rect;         //>@�����С

    int                     m_PixPointer;

    qreal                   m_XDiff,m_YDiff;

    QTimeLine               m_Animator;
    int                     m_AnimateCount;
    int                     m_AnimateTime;
    QTimeLine::CurveShape   m_AnimateType;

public slots:
    virtual void updateEffect(QString pParameter);
    void updateAnimal(int pFrame);
};

#endif // EFFECTANIMAL_H
