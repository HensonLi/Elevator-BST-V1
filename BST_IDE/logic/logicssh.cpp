#include "logicssh.h"


LogicSSH::LogicSSH(QObject *parent)
    :LogicDevice(parent)
{
}

LogicSSH::~LogicSSH()
{
}

quint32 LogicSSH::InitPara()
{
    QDomNodeList tmpParaNodeList = m_ParaElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    quint32 validcount = 0;
    QHBoxLayout* tmpHBoxLayout;
    for(int i=0;i<tmpParaNodeList.count();i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        if(!tmpName.compare("Passphrase"))
        {
            tmpHBoxLayout = CreateTextEdit(tmpName, tmpElement.text());
        }
        else if(!tmpName.compare("Device"))
        {
            QStringList tmpDevList = tmpElement.text().split(';');
            AppendPHDevices(tmpDevList);
            tmpHBoxLayout = CreateCheck(tmpName, tmpElement.attribute("List").split(';'), tmpDevList);
        }
        else
        {
            continue;
        }
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    validcount++;
    return validcount;
}

void LogicSSH::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Passphrase"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyTextEdit(tmpComponent, tmpElement, mPassphrase, tmpComponent->text()))
            {
            }
        }
    }
    else if(!tmpName.compare("Device"))
    {
        CheckList *tmpComponent = VAR_CAST<CheckList *>(pObject);
        if(tmpComponent)
        {
            QStringList tmpDevList = tmpComponent->GetSelectList();
            QStringList tmpList = PHDevicesAdded(tmpDevList, tmpComponent);
            if(!ModifyCheck(tmpComponent, tmpElement, tmpComponent->m_SelectList, tmpList))
            {
            }
        }
    }
}
