#include "logicbase.h"
#ifdef IDE
    #include "devicescene.h"
#elif DISPLAY
    #include "devicemanager.h"
#endif

LogicDevice::LogicDevice(QObject *parent) :
    IODeviceBase(parent)
{
    mPHDevices.clear();
}

LogicDevice::~LogicDevice()
{
    mPHDevices.clear();
}

bool LogicDevice::InitDevice(LG_Type pLGType, QDomElement pElement)
{
    if(pLGType == L_INVALID || pElement.isNull())
        return false;
    mDevType = pLGType;
    mDevName = pElement.tagName();
    if(!pElement.hasChildNodes())
        return false;
    m_ParaElement = pElement;
    //>@��������ҳ��
    if(m_SettingGroup)
        m_SettingGroup->deleteLater();
    m_SettingGroup = new Groupbox(mDevName);
    if(!m_SettingGroup)
        return false;
    //>@��ʼ��������
    quint32 count = InitPara();
    if(!count)
    {
        m_SettingGroup->deleteLater();
        m_SettingGroup = 0;
        return false;
    }
    m_SettingGroup->setFixedHeight(80+count*35);
    return true;
}

QStringList LogicDevice::AppendPHDevices(QStringList pPHDevs)
{
    QStringList tmpList;
    DeletePHDevices();
    for(int i=0;i<pPHDevs.count();i++)
    {
        PH_Type tmpType = GetPHType(pPHDevs.at(i));
        if(tmpType == P_INVALID)
            continue;
#ifdef IDE
        PhysicalDevice *tmpPHDev = m_DeviceScene->m_PHDevList.value(tmpType);
#elif DISPLAY
        PhysicalDevice *tmpPHDev = m_DevManager->m_PHDevList.value(tmpType);
#endif
        if(!tmpPHDev)  //>@�Ƿ���ڴ������豸
            continue;
        //>@�鿴�������豸���Ƿ��д��߼��豸�����û�����Ȳ���
        tmpPHDev->AppendLGDev(mDevType, this);
        connect(tmpPHDev, SIGNAL(readyRead()), this, SLOT(ParseDataStream()));
        connect(tmpPHDev, SIGNAL(sDeviceStateChanged()), this, SLOT(DeviceStateChanged()));
        //>@
        mPHDevices.insert(tmpType, tmpPHDev);
        tmpList.append(tmpPHDev->mDevName);
    }
    return tmpList;
}

QStringList LogicDevice::PHDevicesAdded(QStringList pList, CheckList *pComponent)
{
    if(!pComponent)
        return QStringList();
    QStringList tmpList = AppendPHDevices(pList);
    for(int i=0;i<pList.count();i++)
    {
        QString tmpDevString = pList.at(i);
        if(!tmpList.contains(tmpDevString))
        {
            for(int j=0;j<pComponent->m_CheckboxList.count();j++)
            {
                Checkbox *tmpCheckBox = pComponent->m_CheckboxList.at(j);
                if(!tmpCheckBox)
                    continue;
                if(tmpCheckBox->text().compare(tmpDevString) == 0)
                {
                    tmpCheckBox->setChecked(false);
                    break;
                }
            }
        }
    }
    //>@���������豸�ı���ź�
    emit sPHDeviceChanged();
    return tmpList;
}

//>@pSync��ʾɾ���ڵ�����Ƿ�ͬ����������
void LogicDevice::PHDeviceDeleted(PhysicalDevice *pPHDev, bool pSync)
{
    if(pPHDev == 0)
        return;
    PhysicalDevice* tmpPHDev = mPHDevices.take(pPHDev->mDevType);
    if(!tmpPHDev)
        return;
    if(tmpPHDev != pPHDev)
    {

    }
    //>@ɾ������ڵ��ͬʱ������Ҫ�������ڵ������Լ�������
    tmpPHDev->mLGDevices.remove(mDevType);
    disconnect(tmpPHDev, SIGNAL(readyRead()), this, SLOT(ParseDataStream()));
    disconnect(tmpPHDev, SIGNAL(sDeviceStateChanged()), this, SLOT(DeviceStateChanged()));
    //>@ͬ��������
    if(pSync)
    {
        if(m_SettingGroup)
        {
            CheckList *tmpCheckList = m_SettingGroup->findChild<CheckList *>("Device");
            if(!tmpCheckList)
                return;
            for(int i=0;i<tmpCheckList->m_CheckboxList.count();i++)
            {
                Checkbox *tmpCheckBox = tmpCheckList->m_CheckboxList.at(i);
                if(!tmpCheckBox)
                    continue;
                if(tmpCheckBox->text().compare(tmpPHDev->mDevName) == 0)
                {
                    tmpCheckBox->setChecked(false);
                    return;
                }
            }
        }
    }
}

void LogicDevice::DeletePHDevices()
{
    QList<PH_Type> tmpList = mPHDevices.keys();
    for(int i=0;i<tmpList.count();i++)
    {
        PhysicalDevice* tmpPHDev = mPHDevices.take(tmpList.at(i));
        if(!tmpPHDev)
            continue;
        //>@ɾ������ڵ��ͬʱ������Ҫ�������ڵ������Լ�������
        tmpPHDev->mLGDevices.remove(mDevType);
        disconnect(tmpPHDev, SIGNAL(readyRead()), this, SLOT(ParseDataStream()));
        disconnect(tmpPHDev, SIGNAL(sDeviceStateChanged()), this, SLOT(DeviceStateChanged()));
    }
    mPHDevices.clear();
}

void LogicDevice::SetPHDeviceEditble(bool flag)
{
    if(m_SettingGroup)
    {
        CheckList *tmpCheckList = m_SettingGroup->findChild<CheckList *>("Device");
        if(!tmpCheckList)
            return;
        for(int i=0;i<tmpCheckList->m_CheckboxList.count();i++)
        {
            Checkbox *tmpCheckBox = tmpCheckList->m_CheckboxList.at(i);
            if(!tmpCheckBox)
                continue;
            tmpCheckBox->setEnabled(flag);
        }
    }
}
