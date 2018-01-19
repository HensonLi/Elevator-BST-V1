#include "graphicsfunction.h"

GraphicsFunction::GraphicsFunction(QGraphicsItem *parent, QRectF pRect) :
    GraphicsBase(parent, pRect)
{
}

bool GraphicsFunction::InitEffectRc(STATE_INFO* pEffect)
{
    QString tmpString = pEffect->StateName;
    if(tmpString.isEmpty())
    {
        return false;
    }
    if(tmpString.compare(QString("Common"), Qt::CaseInsensitive) == 0)
    {
        RESOURCE_INFO tmpRc;
        char index;
        int count = pEffect->Effect.RcFiles.count();
        for(int i=0;i<count;i++)
        {
            tmpRc = pEffect->Effect.RcFiles.at(i);
            index = GetKey(tmpRc.RcName);
            IDE_TRACE_INT(index);
            if(LoadPath(index, tmpRc.RcFile) == false)
            {
                IDE_TRACE_STR(tmpRc.RcFile);
            }
        }
    }
    else if(tmpString.compare(QString("Change"), Qt::CaseInsensitive) == 0)
    {
        AREA_ANIMATE *tmpAreaAnimate = new  AREA_ANIMATE;
        if(pEffect->Effect.EffectType.compare(QString("SlipCycle"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeSlipCycle;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Roll"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeRoll;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Transparent"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeTransparent;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Zoom"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeZoom;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Flip"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeFlip;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Rotate"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeRotate;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Cube"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeCube;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Cylinder"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeCylinder;
        }
        else
        {
            delete tmpAreaAnimate;
            return false;
        }
        analysisEffectPara(tmpAreaAnimate, pEffect->Effect.EffectPara);
        m_EffectGroup.insert(OPERATE_CHANGE, tmpAreaAnimate);
    }
    else if(tmpString.compare(QString("Steady"), Qt::CaseInsensitive) == 0)
    {
        AREA_ANIMATE *tmpAreaAnimate = new  AREA_ANIMATE;
        if(pEffect->Effect.EffectType.compare(QString("SlipCycle"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeSlipCycle;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Roll"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeRoll;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Transparent"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeTransparent;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Zoom"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeZoom;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Flip"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeFlip;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Rotate"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeRotate;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Cube"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeCube;
        }
        else if(pEffect->Effect.EffectType.compare(QString("Cylinder"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeCylinder;
        }
        else
        {
            delete tmpAreaAnimate;
            return false;
        }
        //>@������ʱʱ���޸���Ч����
        tmpAreaAnimate->mLoopCount = 0;
        analysisEffectPara(tmpAreaAnimate, pEffect->Effect.EffectPara);
        m_EffectGroup.insert(OPERATE_STEADY, tmpAreaAnimate);
    }
    else
    {
        return false;
    }
    return true;
}

bool GraphicsFunction::analysisEffectParaItem(AREA_ANIMATE* pAreaEffect, QString pLabel, QString pContent)
{
    return GraphicsBase::analysisEffectParaItem(pAreaEffect, pLabel, pContent);
}

bool GraphicsFunction::Start()
{
    if(GraphicsBase::Start() == false)
    {
        IDE_TRACE();
        return false;
    }
    m_RegionState = INVALIDRC;
    return true;
}

bool GraphicsFunction::PaintEffect(QPainter *p)
{
    if(GraphicsBase::PaintEffect(p) == false)
        return false;
    switch(m_AreaAnimate->mEffectType)
    {
        case EffectTypeRoll:
            Paint2D_Roll(p, rect());
            break;
        case EffectTypeSlipCycle:
            Paint2D_Slipcycle(p, rect());
            break;
        case EffectTypeTransparent:
            Paint2D_Transparent(p, rect());
            break;
        case EffectTypeZoom:
            Paint2D_Zoom(p, rect());
            break;
        case EffectTypeFlip:
            Paint2D_Flip(p, rect());
            break;
        case EffectTypeRotate:
            Paint2D_Rotate(p, rect());
            break;
#ifdef  SURRPORT_3D
        case EffectTypeCube:
            Paint3D_Cube(p, rect());
            break;
        case EffectTypeCylinder:
            Paint3D_Cylinder(p, rect());
            break;
#endif
        default:
            Paint2D_None(p, rect());
            break;
    }
    return true;
}

void GraphicsFunction::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    GraphicsBase::updateEffect(pOperate, pPara);
    switch(pOperate)
    {
        case OPERATE_KEYPRESS:
        {
            //>@�ֽ����꣬�������ϰ����������Ϲ���
            QPoint tmpPoint = pPara.toPoint();
            if(tmpPoint.y() < size().height()/2)
            {
                updateEffect(getPrevRc(m_RegionState), OPERATE_STEADY);
            }
            else if(tmpPoint.y() > size().height()/2)
            {
                updateEffect(getNextRc(m_RegionState), OPERATE_STEADY);
            }
            break;
        }
        case OPERATE_CHANGE:
        {
            int index = pPara.toInt();
            //>@�������0xffff����Ĭ�ϲ�����һ��
            if(index == NEXTRC)
                index = getNextRc(m_RegionState);
            else if(index == PREVRC)
                index = getPrevRc(m_RegionState);
            updateEffect(index, OPERATE_STEADY);
            break;
        }
        default:
            break;
    }
}

void GraphicsFunction::updateEffect(int pState, AREA_OPERATE pOperate)
{
    bool tmpStateChange = false;
    bool tmpOperateChange = false;
    if(pState != m_RegionState)
        tmpStateChange = true;
    if(pOperate != m_OperateInfo[STEP1].mOperate)
        tmpOperateChange = true;
    if(tmpStateChange == false && tmpOperateChange == false)
    {
        IDE_TRACE();
        return;
    }

    m_OperateInfo[STEP0].mValid = false;
    m_OperateInfo[STEP1].mValid = false;
    m_Animator.stop();
    if(tmpStateChange)
    {
        m_OperateInfo[STEP0].mValid = true;
        m_OperateInfo[STEP0].mOperate = OPERATE_CHANGE;
        m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer;
        m_PixmapPointer.mCurPointer = GetPixmap(pState, size());
    }
    m_OperateInfo[STEP1].mValid = true;
    m_OperateInfo[STEP1].mOperate = pOperate;
    m_RegionState = pState;
    //>@ִ��STEP0�е�����
    OperateStep0();
}
