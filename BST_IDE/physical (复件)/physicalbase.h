#ifndef PHYSICALBASE_H
#define PHYSICALBASE_H

#include "iodevicebase.h"

#ifdef IDE
    class DeviceScene;
    class DeviceItem;
#elif DISPLAY
    class DeviceManager;
#endif
class LogicDevice;

class PhysicalDevice : public IODeviceBase
{
    Q_OBJECT
public:
    PhysicalDevice(QObject *parent = 0);
    ~PhysicalDevice();

    virtual bool copy(QString pSrc, QString pDst)  {return true;}
    virtual bool cut(QString pSrc, QString pDst)  {return true;}
    virtual bool del(QString pPath)  {return true;}

    virtual bool mkdir(QString pPath)  {return true;}
    virtual bool copydir(QDir pSrc, QDir pDst, bool subfolder)  {return true;}
    virtual bool movedir(QString pSrc, QString pDst, bool subfolder)  {return true;}
    virtual bool deldir(QString pDir)  {return true;}
    virtual bool cleardir(QString pDir)  {return true;}

    virtual void SetDevState(DEVICESTATE pState, QString pDevPath);

    bool InitDevice(PH_Type pPHType, QDomElement pElement);
    void AppendLGDev(LG_Type pType, LogicDevice *pDevice);

    //>@��ȡ���ڵ��е���Ҫ��Ϣ
    virtual QString GetInformation() { return QString("%1:%2").arg(mDevName).arg(mDevPath); }

signals:
    void sDeviceStateChanged();

public:
    QHash<LG_Type, LogicDevice*> mLGDevices;       //>@Э�����ͽڵ��

    QDomNodeList m_ParaElementList;
    DEVICESTATE  mDevState;     //>@�豸״̬(����/����)
    QString      mDevPath;      //>@�豸·��:�豸�������߿ɷ��ʵ�ַ
    PH_Type      mDevType;
    bool         mCon;          //>@�Ƿ�������
};

#endif // PHYSICALBASE_H
