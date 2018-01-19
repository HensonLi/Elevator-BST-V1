#include "graphicsmessage.h"

GraphicsMessage::GraphicsMessage(QGraphicsItem *parent) :
    GraphicsOperate(parent)
{
}

bool GraphicsMessage::Start()
{
    GraphicsOperate::Start();
    connect(&m_Animator, SIGNAL(finished()), this, SLOT(slot_Finished()));
    //>@Ĭ������Message����
#ifndef IDE
    hide();
#endif
    return true;
}

void GraphicsMessage::slot_Finished()
{
    hide();
}

void GraphicsMessage::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_KEYPRESS:
        {
            //>@�ֽ����꣬�������ϰ����������Ϲ���
            QPoint tmpPoint = pPara.toPoint();
            if(tmpPoint.y() < size().height()/2)
            {
                info(QString("Test Info"), 1000);
            }
            else if(tmpPoint.y() > size().height()/2)
            {
                warn(QString("Test Warn"), 1000);
            }
            break;
        }
        default:
            break;
    }
}

void GraphicsMessage::updateEffect(MSG_INFO pMsgInfo)
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
    m_PixmapPointer.mLastPointer = LoadMessage(pMsgInfo.mMsgLevel, pMsgInfo.mMessage, size());
    m_PixmapPointer.mCurPointer = m_PixmapPointer.mLastPointer;
    //>@��ʾ��Ϣ���ڣ���ִ��STEP0�е�����
    if(isVisible() == false)
        show();

    //>@
    m_MsgInfo.mMsgLevel = pMsgInfo.mMsgLevel;
    m_MsgInfo.mMessage = pMsgInfo.mMessage;
    m_MsgInfo.mDelay = pMsgInfo.mDelay;

    OperateStep0();
}

QPixmap GraphicsMessage::LoadMessage(MSG_LEVEL pMsgLevel, QString pContent, QSizeF pSize)
{
    if(pContent.isEmpty())
        return QPixmap();
    QImage tmpImage(pSize.width(), pSize.height(), QImage::Format_ARGB32_Premultiplied);
    if(tmpImage.isNull())
        return false;
    tmpImage.fill(Qt::transparent); //>@����͸��ͼ��
    QPainter p(&tmpImage);
    QPixmap tmpPixmap = GetPixmap(pMsgLevel);
    if(tmpPixmap.isNull() == false)
    {
        p.drawPixmap(0, 0, pSize.width(), pSize.height(), tmpPixmap);
    }
    //>@�������ŵ��ı�����λ��
    QRectF tmpRect = rect();
    QColor tmpColor;
    switch(pMsgLevel)
    {
        case LEVEL_INFO:
            tmpColor = Qt::black;
            break;
        case LEVEL_WARN:
            tmpColor = Qt::darkMagenta;
            break;
        case LEVEL_ERROR:
            tmpColor = Qt::red;
            break;
        default:
            return QPixmap();
    }
    WriteText(&p, pContent, QRect(tmpRect.x()+20,tmpRect.y()+40,tmpRect.width()-40,tmpRect.height()-80), QString("Arial"), tmpColor, 15);
    p.end();
    return QPixmap::fromImage(tmpImage);
}

void GraphicsMessage::info(QString pContent, int pDelay)
{
    MSG_INFO tmpMsgInfo;
    tmpMsgInfo.mMsgLevel = LEVEL_INFO;
    tmpMsgInfo.mMessage = pContent;
    tmpMsgInfo.mDelay = pDelay;
    updateEffect(tmpMsgInfo);
}

void GraphicsMessage::warn(QString pContent, int pDelay)
{
    MSG_INFO tmpMsgInfo;
    tmpMsgInfo.mMsgLevel = LEVEL_WARN;
    tmpMsgInfo.mMessage = pContent;
    tmpMsgInfo.mDelay = pDelay;
    updateEffect(tmpMsgInfo);
}

void GraphicsMessage::error(QString pContent, int pDelay)
{
    MSG_INFO tmpMsgInfo;
    tmpMsgInfo.mMsgLevel = LEVEL_ERROR;
    tmpMsgInfo.mMessage = pContent;
    tmpMsgInfo.mDelay = pDelay;
    updateEffect(tmpMsgInfo);
}


DesignMessage::DesignMessage(QGraphicsItem *parent) :
    GraphicsMessage(parent)
{
    QAction *tmpDefault = m_ActionGroup->addAction(tr("INFO"));
    m_ActionGroup->addAction(tr("WARN"));
    m_ActionGroup->addAction(tr("ERROR"));
    tmpDefault->setChecked(true);
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

void DesignMessage::ExecAction(QAction *pAction)
{
    updateEffect(OPERATE_CHANGE, QVariant(GetMsgLevel(pAction->text())));
}

