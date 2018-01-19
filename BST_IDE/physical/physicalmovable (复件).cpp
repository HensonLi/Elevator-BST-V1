#include "physicalmovable.h"

PhysicalMovable::PhysicalMovable(QObject *parent):
    PhysicalDevice(parent)
{
    mIsMovable = false;
}

PhysicalMovable::~PhysicalMovable()
{
}

quint32 PhysicalMovable::InitPara()
{
    QDomNodeList tmpParaNodeList = m_ParaElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    PreInitProcess();
    quint32 validcount = 0;
    for(int i=0;i<tmpParaNodeList.count()-1;i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QHBoxLayout* tmpHBoxLayout = 0;
        QString tmpName = tmpElement.tagName();
        if(!tmpName.compare("Movable"))
        {
            QString tmpValue = tmpElement.text();
            mIsMovable = (tmpValue.compare("true",Qt::CaseInsensitive)==0)?true:false;
            tmpHBoxLayout = CreateBool(tmpName, tmpValue);
        }
        else if(!InitSubPHPara(tmpName, tmpElement, tmpHBoxLayout))
        {
            continue;
        }
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    if(!mIsMovable)
    {
        SetPlugged();
        mDevState = S_PLUGGED;
    }
    //>@����
    PostInitProcess();
    return validcount;
}

void PhysicalMovable::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    PreUpdateProcess();
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Movable"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mIsMovable, tmpComponent->isChecked()))
            {
                if(!mIsMovable)
                {
                    SetPlugged();
                    mDevState = S_PLUGGED;
                }
            }
        }
    }
    else
    {
        UpdateSubPHPara(tmpName, pObject, tmpElement);
    }
    //>@����
    PostUpdateProcess();
}

//>@�����κο��ƶ��豸��ֻ������һ����豸������ٲ�����ͬ���豸��ϵͳ�����д���
//>@���γ��豸ʱ�������жϴ��豸��·����mDevPath�Ƿ���ͬ���������ͬ������Ϊ���ǵ�ǰ��Ч�豸���γ�
void PhysicalMovable::slot_DevStateChanged(DEVICESTATE pState, QString pDevPath)
{
    if(!mIsMovable)  //>@�����ǰ�豸���ǿɲ���豸����ִ�д˺�����Ч
        return;
    if(mDevState != pState)  //>@�鿴�Ƿ��Ѿ����ڿɲ���豸������ǣ����²�����豸������
    {
        bool ok = false;
        switch(pState)
        {
            case S_UNPLUGGED:
            {
                if(mDevPath.compare(pDevPath))
                    return;
                ok = SetUnplugged();
                break;
            }
            case S_PLUGGED:
            {
                ok = SetPlugged();
                if(ok)
                    mDevPath = pDevPath;
                break;
            }
            default:
                break;
        }
        if(ok)
            mDevState = pState;
    }
}

