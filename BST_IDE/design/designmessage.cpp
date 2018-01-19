#include "designmessage.h"

DesignMessage::DesignMessage(QGraphicsItem *parent, QRectF pRect) :
    DesignBase(parent, pRect)
{
    QAction *tmpDefault = m_ActionGroup->addAction(tr("INFO"));
    m_ActionGroup->addAction(tr("WARN"));
    m_ActionGroup->addAction(tr("ERROR"));
    tmpDefault->setChecked(true);
}

bool DesignMessage::InitEffectRc(STATE_INFO* pEffect)
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
        if(pEffect->Effect.EffectType.compare(QString("Transparent"), Qt::CaseInsensitive) == 0)
        {
            tmpAreaAnimate->mEffectType = EffectTypeTransparent;
        }
        else
        {
            delete tmpAreaAnimate;
            return false;
        }
        analysisEffectPara(tmpAreaAnimate, pEffect->Effect.EffectPara);
        m_EffectGroup.insert(OPERATE_CHANGE, tmpAreaAnimate);
    }
    else
    {
        return false;
    }
    return true;
}

bool DesignMessage::analysisEffectParaItem(AREA_ANIMATE* pAreaEffect, QString pLabel, QString pContent)
{
    return DesignBase::analysisEffectParaItem(pAreaEffect, pLabel, pContent);
}

bool DesignMessage::Start()
{
    DesignBase::Start();
    return true;
}

MSG_LEVEL GetMsgLevel(QString pLevel)
{
    if(pLevel.isEmpty())
        return LEVEL_INVALID;
    if(pLevel.compare("INFO") == 0)
    {
        return LEVEL_INFO;
    }
    else if(pLevel.compare("WARN") == 0)
    {
        return LEVEL_WARN;
    }
    else if(pLevel.compare("ERROR") == 0)
    {
        return LEVEL_ERROR;
    }
    return LEVEL_INVALID;
}

QList<QAction*> DesignMessage::GetActionList()
{
    QList<QAction*> tmpList;
    if(m_ActionGroup)
    {
        tmpList = m_ActionGroup->actions();
        //>@���ݵ�ǰ���е���Դʹ����Ӧ����Action
        for(int i=0;i<tmpList.count();i++)
        {
            QAction *tmpAction = tmpList.at(i);
            if(tmpAction == 0)
                continue;
            tmpAction->setEnabled(true);
        }
    }
    return tmpList;
}

void DesignMessage::ExecAction(QAction *pAction)
{
    updateEffect(OPERATE_CHANGE, QVariant(GetMsgLevel(pAction->text())));
}

bool DesignMessage::PaintEffect(QPainter *p)
{
    if(m_AreaAnimate == 0)
    {
        Paint2D_None(p, rect());
    }
    else
    {
        switch(m_AreaAnimate->mEffectType)
        {
            case EffectTypeTransparent:
                Paint2D_Transparent(p, rect());
                break;
            default:
                break;
        }
    }
    return true;
}

void DesignMessage::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_CHANGE:
        {
            int tmpDelay = 1000;
            AREA_ANIMATE* tmpAreaAnimate =  m_EffectGroup.value(OPERATE_CHANGE);
            if(tmpAreaAnimate)
            {
                tmpDelay = tmpAreaAnimate->mAnimateTime;
            }
            MSG_LEVEL tmpLevel = (MSG_LEVEL)(pPara.toInt());
            switch(tmpLevel)
            {
                case LEVEL_INFO:
                {
                    info(QString("This is the Test message for Information!"), tmpDelay);
                    break;
                }
                case LEVEL_WARN:
                {
                    warn(QString("This is the Test message for Warn!"), tmpDelay);
                    break;
                }
                case LEVEL_ERROR:
                {
                    error(QString("This is the Test message for Error!"), tmpDelay);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}

void DesignMessage::updateEffect(MSG_INFO pMsgInfo)
{
    if(pMsgInfo.mMessage.isEmpty())
    {
        m_Animator.stop();
        hide();
        return;
    }
    m_OperateInfo[STEP0].mValid = false;
    m_OperateInfo[STEP1].mValid = false;
    m_Animator.stop();
    m_OperateInfo[STEP0].mValid = true;
    m_OperateInfo[STEP0].mOperate = OPERATE_CHANGE;
    //>@������ʱʱ���޸���Ч����
    AREA_ANIMATE* tmpAreaAnimate =  m_EffectGroup.value(OPERATE_CHANGE);
    if(tmpAreaAnimate)
    {
        tmpAreaAnimate->mAnimateTime = pMsgInfo.mDelay;
        tmpAreaAnimate->mFrameStart = 0;
        tmpAreaAnimate->mFrameEnd = pMsgInfo.mDelay/50;
    }
    //>@������ʾ��Դ
    m_PixmapPointer.mLastPointer = LoadMessage(pMsgInfo.mMsgLevel, pMsgInfo.mMessage, size().toSize());
    //m_PixmapPointer.mCurPointer = QPixmap();
    //>@��ʾ��Ϣ���ڣ���ִ��STEP0�е�����
    if(isVisible() == false)
        show();
    OperateStep0();
}

QPixmap DesignMessage::LoadMessage(MSG_LEVEL pMsgLevel, QString pContent, QSize pSize)
{
    if(pContent.isEmpty())
        return QPixmap();
    QImage tmpImage(pSize.width(), pSize.height(), QImage::Format_ARGB32_Premultiplied);
    if(tmpImage.isNull())
        return false;
    tmpImage.fill(Qt::transparent); //>@����͸��ͼ��
    QPainter p(&tmpImage);
    p.drawPixmap(0, 0, pSize.width(), pSize.height(), GetPixmap(pMsgLevel));
    //>@�������ŵ��ı�����λ��
    QRectF tmpRect = rect();
    QColor tmpColor;
    switch(pMsgLevel)
    {
        case LEVEL_INFO:
            tmpColor = Qt::black;
            break;
        case LEVEL_WARN:
            tmpColor = Qt::blue;
            break;
        case LEVEL_ERROR:
            tmpColor = Qt::red;
            break;
        default:
            return QPixmap();
    }
    WriteText(&p, pContent, QRect(tmpRect.x()+20,tmpRect.y()+40,tmpRect.width()-40,tmpRect.height()-80), tmpColor, 15);
    p.end();
    return QPixmap::fromImage(tmpImage);
}

void DesignMessage::info(QString pContent, int pDelay)
{
    MSG_INFO tmpMsgInfo;
    tmpMsgInfo.mMsgLevel = LEVEL_INFO;
    tmpMsgInfo.mMessage = pContent;
    tmpMsgInfo.mDelay = pDelay;
    updateEffect(tmpMsgInfo);
}

void DesignMessage::warn(QString pContent, int pDelay)
{
    MSG_INFO tmpMsgInfo;
    tmpMsgInfo.mMsgLevel = LEVEL_WARN;
    tmpMsgInfo.mMessage = pContent;
    tmpMsgInfo.mDelay = pDelay;
    updateEffect(tmpMsgInfo);
}

void DesignMessage::error(QString pContent, int pDelay)
{
    MSG_INFO tmpMsgInfo;
    tmpMsgInfo.mMsgLevel = LEVEL_ERROR;
    tmpMsgInfo.mMessage = pContent;
    tmpMsgInfo.mDelay = pDelay;
    updateEffect(tmpMsgInfo);
}
